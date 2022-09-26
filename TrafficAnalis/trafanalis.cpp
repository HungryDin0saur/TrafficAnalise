#include "trafanalis.h"
#include "ui_trafanalis.h"

TrafAnalis::TrafAnalis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafAnalis)
{
    ui->setupUi(this);

    ui->informBoard->setFontPointSize(13);
}

TrafAnalis::~TrafAnalis()
{
    delete ui;
}

void TrafAnalis::on_pushButton_clicked()
{
    ui->procDow->setVisible(true);
    ui->informBoard2->clear();
    ui->informBoard2->setFontPointSize(13);

    int ipVecSize = 1;

    QString dir = "D:/MY/Work!/MyProgect/MDIG_practice/";
    QFile *file = new QFile(dir + "out1.bin");
    file->open(QFile::ReadOnly);

    fsize = file->size();
    ui->procDow->setMaximum((int)(fsize-1));
    ui->informBoard2->append("Всего байт: " + QString::number(fsize));
    ui->informBoard2->insertPlainText("\n");

    bytes = file->readAll();

    file->close();
    file->deleteLater();

    QByteArray pacLen;
    QString ipBuf;
    pacLen.resize(2);

    ipVec.resize(10000);

    for(qint64 i=0;i<fsize;)
    {
       pacLen = bytes.mid((i+1), 1);

       pacLen += bytes.mid(i, 1);

       ui->informBoard2->insertPlainText("Packet lenght: " + QString::number(pacLen.toHex().toInt(nullptr, 16)));
       ui->informBoard2->insertPlainText("\n");
       i+=2;
       ui->informBoard2->insertPlainText("MAC dst: " + QString::fromStdString(bytes.mid(i, 6).toHex().toStdString()));
       ui->informBoard2->insertPlainText("\n");
       i+=6;
       ui->informBoard2->insertPlainText("MAC src: " + QString::fromStdString(bytes.mid(i, 6).toHex().toStdString()));
       ui->informBoard2->insertPlainText("\n");
       i+=20;

       ipBuf = QString::number(bytes.mid(i, 1).toHex().toInt(nullptr, 16));
       for(int j=(i+1);j<(i+4);j++)
       {
           ipBuf += ('_');
           ipBuf += (QString::number(bytes.mid(j, 1).toHex().toInt(nullptr, 16)));
       }
       ui->informBoard2->insertPlainText("IP dst: " + ipBuf);
       ui->informBoard2->insertPlainText("\n");
       i+=4;

       ipBuf = QString::number(bytes.mid(i, 1).toHex().toInt(nullptr, 16));
       for(int j=(i+1);j<(i+4);j++)
       {
           ipBuf += ('_');
           ipBuf += (QString::number(bytes.mid(j, 1).toHex().toInt(nullptr, 16)));
       }
       ui->informBoard2->insertPlainText("IP src: " + ipBuf);
       ui->informBoard2->insertPlainText("\n");
       ui->informBoard2->insertPlainText("\n");

       i-=32;

       for(int j=0;j<ipVecSize;j++)
       {
           if(j==(ipVecSize-1))
           {
               ipVec[ipVecSize].ipSRC = ipBuf;
               ipVec[ipVecSize].dir = ipVec[ipVecSize].ipSRC  + ".txt";

               filePac = new QFile(dir + ipVec[ipVecSize].dir);
               filePac->open(QFile::WriteOnly);
               QDataStream *stream1 = new QDataStream(filePac);

               *stream1 <<  bytes.mid(i, (pacLen.toHex().toInt(nullptr, 16)+2));

               filePac->close();
               filePac->destroyed(this);
               filePac->deleteLater();

               ipVecSize++;

               break;
           }
           else
           {
              qDebug() << filePac->fileName() << "-----" << ipVec[j].dir;
               if(ipBuf == ipVec[j].ipSRC)
               {
                   filePac = new QFile(dir + ipVec[j].dir);
                   filePac->open(QFile::Append);
                   QDataStream *stream1 = new QDataStream(filePac);

                   *stream1 << bytes.mid(i, (pacLen.toHex().toInt(nullptr, 16)+2));

                   filePac->close();
                   filePac->destroyed(this);
                   filePac->deleteLater();

                   break;
               }
           }
       }

       i+=(pacLen.toHex().toInt(nullptr, 16)+2);

       ui->procDow->setValue(i+(pacLen.toHex().toInt(nullptr, 16)+2));

       qApp->processEvents();
    }

    bytes.clear();
}

void TrafAnalis::on_pushButton_2_clicked()
{
        bytes.clear();

        ui->informBoard->setFontPointSize(16);

        ui->procDow->setVisible(true);

        QString dir = "D:/MY/Work!/MyProgect/MDIG_practice/";
        QFile *file = new QFile(dir + "out2.bin");
        if(!file->open(QFile::ReadOnly))
        {
            QMessageBox::information(this, "Error", file->errorString());
            return;
        }

        QByteArray pacLen;

        fsize = file->size();
        ui->procDow->setMaximum((int)(fsize-1));
        ui->informBoard->append("Всего байт: " + QString::number(fsize));
        ui->informBoard->insertPlainText("\n");

        bytes = file->readAll();

        file->close();
        file->deleteLater();

        defStream = new QFile();
        QByteArray defragmentPacLen;
        quint16 pl = 0;

        for(qint64 i=0;(i<fsize) && (stop==false);)
        {
           pacLen = bytes.mid((i+1), 1);

           pacLen += bytes.mid(i, 1);

           ui->informBoard->insertPlainText("Packet lenght: " + QString::number(pacLen.toHex().toInt(nullptr, 16)+2));
           ui->informBoard->insertPlainText("\n");

           if(bytes.mid((i+4), 1).toHex() == "08")
           {

             ip = "_";
             for(qint64 j=(i+18);j<(i+22);j++)
             {
                 ip += QString::number(bytes.mid(j, 1).toHex().toInt(nullptr, 16));
                 ip += ".";
             }
           /*
             bits.resize(8);
             for (int b = 0; b < 8; ++b)
             {
                bits.setBit(b, (bytes.at(i+6) >> b) & 1);
             }
             qDebug() << bits;
             bits.clear();
           */
              defStream->setFileName(defPath + "1_" + QString::number(fragmentCounter) + "NoDefrag" + ip +  + ".bin");
              defStream->open(QFile::WriteOnly);
              defStream->write(pacLen.mid(1), 1);
              defStream->write(pacLen.mid(0), 1);
              defStream->write(bytes.mid((i+7), (pacLen.toHex().toInt(nullptr, 16)+2)));
              fragmentCounter++;

               ip.clear();
           }
           else
           {
               bits.resize(8);
               for (int b = 0; b < 8; ++b)
               {
                  bits.setBit(b, (bytes.at(i+6) >> b) & 1);
               }

               if(bits.at(6)==0 && bits.at(7)==1)
               {
                   pl = (pacLen.toHex().toInt(nullptr, 16)-5);
                   for(qint64 j=i; j<(i+(pacLen.toHex().toInt(nullptr, 16)));j++)
                   {
                       if((bytes.mid(j, 1).toHex() == "45") && (bytes.mid((j+1), 1).toHex() == "00"))
                       {
                           ip += ('_');
                           j+=12;
                           int d = (j+4);
                           while(j<d)
                           {
                              ip += (QString::number(bytes.mid(j, 1).toHex().toInt(nullptr, 16)));
                              ip += ('.');
                              j++;
                           }
                           ip += ("_");
                           break;
                       }
                   }

                  qDebug() << pl << pacLen.toHex(' ') << bits << " " << "New packet";

                  buff.append(bytes.mid((i+7), (pacLen.toHex().toInt(nullptr, 16)+2)));
               }
               else
               {
                   if(bits.at(6)==0 && bits.at(7)==0)
                   {
                       pl += (pacLen.toHex().toInt(nullptr, 16)-5);
                       buff.append(bytes.mid((i+7), (pacLen.toHex().toInt(nullptr, 16)+2)));
                       qDebug() << pl << pacLen.toHex(' ') << bits << " " << bytes.mid((i+4), 1).toHex();
                   }
                   else
                   {
                       pl += (pacLen.toHex().toInt(nullptr, 16)-5);

                       buff.append(bytes.mid((i+7), (pacLen.toHex().toInt(nullptr, 16)+2)));

                       defStream->setFileName(defPath + "2_" + QString::number(fragmentCounter) + "Defragment_" + ip.mid(1,19) + ".bin");
                       defStream->open(QFile::WriteOnly);

                       QDataStream stream(defStream);
                       stream.setByteOrder(QDataStream::LittleEndian);
                       stream << pl;
                       qDebug() << pl << "exit" << pacLen.toHex(' ') << bits << " " << bytes.mid((i+4), 1).toHex();
                       qDebug() << "***********************************************";

                       defStream->write(buff);

                       sizeBute.clear();

                       defragmentCounter++;
                       pl=0;
                       ip.clear();
                       buff.clear();
                   }
               }
               bits.clear();
           }

           ui->informBoard->insertPlainText("\n");
           ui->informBoard->insertPlainText("\n");
           i+=(pacLen.toHex().toInt(nullptr, 16)+2);
           ui->procDow->setValue(i+(pacLen.toHex().toInt(nullptr, 16)+2));
           qApp->processEvents();

         }

        defStream->close();
        defStream->deleteLater();

        bytes.clear();
}

void TrafAnalis::on_pushButton_3_clicked()
{
    ui->pushButton->setEnabled(true);
}
