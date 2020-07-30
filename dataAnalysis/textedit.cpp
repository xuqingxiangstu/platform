#include "textedit.h"
#include <QtWidgets>
#include <QFont>
#include <QColor>
#include <QPainter>

TextEdit::TextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{   
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setLineWrapMode(QPlainTextEdit::NoWrap);

}


TextEdit::~TextEdit()
{

}


int TextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


void TextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{

    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void TextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


void TextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QTextBlock block = firstVisibleBlock();
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(30,60,90));
    QFont font = painter.font();
    painter.setPen(Qt::gray);

    int top;

    do
    {
        if(block == textCursor().block())
        {
            painter.setPen(Qt::yellow);
            font.setBold(true);
        }
        else
        {
            font.setBold(false);
        }

        painter.setFont(font);

        int blockNumber = block.blockNumber();
        top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + (int) blockBoundingRect(block).height();

        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),Qt::AlignRight, number);
            painter.setPen(Qt::gray);
        }
    }

    while((block = block.next()).isValid() && top < viewport()->height());
}


void TextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);       
    }

    setExtraSelections(extraSelections);
}




