#include <QCoreApplication>

#include <QFile>

#include <QDebug>

/** 数据转换结构体 **/
typedef union
{
    unsigned char u8Buf[8]; ///< 无符号8位数组
    char i8Buf[8];          ///< 有符号8位数组
    unsigned char u8Value;  ///< 无符号8位
    unsigned short u16Value;///< 无符号16位
    unsigned int u32Value;  ///< 无符号32位
    unsigned long long u64Value; ///< 无符号64位
    char i8Value; ///< 有符号8位
    short i16Value; ///< 有符号16位
    int i32Value; ///< 有符号32位
    long long i64Value; ///< 有符号64位

    float f32Value; ///< 32位浮点
    double d64Value; ///< 64位浮点
}unDataConvert;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if 0
    QByteArray msg;
    //组帧
    unDataConvert convert;
    //赋值
    convert.f32Value = 230.2;

    //取数(小端模式)
    msg.append(convert.u8Buf[0]);
    msg.append(convert.u8Buf[1]);
    msg.append(convert.u8Buf[2]);
    msg.append(convert.u8Buf[3]);
    //大端模式
    msg.append(convert.u8Buf[3]);
    msg.append(convert.u8Buf[2]);
    msg.append(convert.u8Buf[1]);
    msg.append(convert.u8Buf[0]);
    //end
    //解析
    unDataConvert pConvert;
    //小端
    pConvert.u8Buf[0] = msg.at(0);
    pConvert.u8Buf[1] = msg.at(1);
    pConvert.u8Buf[2] = msg.at(2);
    pConvert.u8Buf[3] = msg.at(3);
    //大端
    pConvert.u8Buf[0] = msg.at(3);
    pConvert.u8Buf[1] = msg.at(2);
    pConvert.u8Buf[2] = msg.at(1);
    pConvert.u8Buf[3] = msg.at(0);
    //取数
    printf("%f", pConvert.f32Value);
#endif
#if 0
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

#endif
    return a.exec();
}
