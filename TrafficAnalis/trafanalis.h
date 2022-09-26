#ifndef TRAFANALIS_H
#define TRAFANALIS_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QTextBrowser>
#include <QByteArray>
#include <QVector>
#include <QVectorIterator>
#include <QBitArray>
#include <QLinkedList>
#include <QDataStream>

namespace Ui {
class TrafAnalis;
}

class TrafAnalis : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrafAnalis(QWidget *parent = nullptr);
    ~TrafAnalis();
public:

    qint64 fsize;

    unsigned char ucbuf;
    QString outBuf;

    struct IP
    {
        QString dir = "";
        QString ipSRC = "";
        QString ipDST = "";
    };

    QVector<IP> ipVec;

    char cbuf;
    int counter;

    std::vector<unsigned char> traf;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::TrafAnalis *ui;

    QVector<QFile> fileVec;
    QVector<QDataStream> fileStream;

    qint64 pacCounter = 0;
    qint64 lenCounter = 0;
    qint64 vecCounter = 1;

private:
    QByteArray bytes;
    QBitArray bits;

    QFile *filePac;

    qint64 packetLen;

    bool count = true;

    QFile *defStream;

    bool stop = false;

    qint64 defragmentCounter = 0, fragmentCounter = 0;

    QString defPath = "D:/MY/Work!/MyProgect/MDIG_practice/PascetToTwoPractice/";

    QString ip;

    QByteArray sizeBute, buff;
};

#endif // TRAFANALIS_H
