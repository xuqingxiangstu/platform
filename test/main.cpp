#include <QCoreApplication>

#include <QFile>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString tmp = " ";

    QByteArray tB = tmp.toUtf8();

    qDebug() << tmp.right(1);
    QString ss = "\n";
    QByteArray sb = ss.toUtf8();
    qDebug() << ss;
    qDebug() << tmp.replace("\\", "");

    QString filePath = "E:/work/P13-platformLibrary/project/1234/data/09.37.01.692.dat";

    QFile fileObj(filePath);
    if(!fileObj.open(QIODevice::ReadOnly))
    {
        QString errInfo = "[ERROR][analysis] open file faild => " + filePath;

    }

    QByteArray allData = fileObj.readAll();

    //step1：分割帧
    QString tmpAllData = allData;

    QString sMark = "\n";

    QStringList frames = tmpAllData.split(sMark, QString::SkipEmptyParts, Qt::CaseSensitive);


    return a.exec();
}
