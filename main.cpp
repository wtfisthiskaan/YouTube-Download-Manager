#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QMessageBox>

QNetworkAccessManager *manager;
QLabel *videoInfoLabel;
QPushButton *downloadButton;
QLabel *progressLabel;

void fetchVideoInfo(const QString &videoLink, QWidget *window){
    QUrl url("http://127.0.0.1:5000/get_video_info");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject json;
    json["video_link"] = videoLink;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    QObject::connect(reply,&QNetworkReply::finished,[reply,window](){
        if(reply->error() == QNetworkReply::NoError){
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonDoc.object();
            QString videoTitle = jsonObject["title"].toString();
            videoInfoLabel->setText("Title: "+videoTitle);
            downloadButton->show();
        }
        else{
            qDebug()<<"Error fetching video Info :"<<reply->errorString();
        }
        reply->deleteLater();
    });
}

void downloadVideo(const QString &videoLink,QWidget *window){
    QUrl url("http://127.0.0.1:5000/download_video");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject json;
    json["video_link"] = videoLink;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    QObject::connect(reply,&QNetworkReply::downloadProgress,[reply](){
        while(reply->canReadLine()){
            QString line = reply->readLine();
            if(line.startsWith("data:")){
                QString jsonText = line.mid(5).trimmed();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8());
                QJsonObject jsonObject = jsonDoc.object();

                if(jsonObject.contains("error")){
                    qDebug()<<"error downloading video"<<jsonObject["error"].toString();
                }
                else{
                    double downloaded = jsonObject["downloaded"].toDouble();
                    double total = jsonObject["total"].toDouble();
                    double speed = jsonObject["speed"].toDouble();
                    int eta = jsonObject["eta"].toInt();

                    QString percent = jsonObject["percent"].toString();

                    QString progressText = QString("%1 of %2 MiB at %3 KiB/s ETA %4s")
                                                .arg(downloaded,0,'f',2)
                                                .arg(total,0,'f',2)
                                                .arg(speed,0,'f',2)
                                                .arg(eta);

                    progressLabel->setText(progressText);
                }
            }
        }
    });

    QObject::connect(reply,&QNetworkReply::finished,[reply,window](){
        if(reply->error()==QNetworkReply::NoError){
            QMessageBox::information(window,"Download Complete","Video Downloaded Success");
        }
        else{
            qDebug()<<"error downloading video"<<reply->errorString();
        }
        reply->deleteLater();
    });
}

// https://www.youtube.com/watch?v=C24m5WEYWxE


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("Youtube Download Manager");
    mainWindow.resize(600,400);

    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *titleLabel = new QLabel("Youtube Download Manager");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size:28px; color: red");
    layout->addWidget(titleLabel);



    QLabel *instructionLabel = new QLabel("Please Input Youtube Video Link");
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size:20px; color: green");
    layout->addWidget(instructionLabel);



    QTextEdit *linkInput = new QTextEdit();
    linkInput->setFixedSize(500,45);
    linkInput->setStyleSheet("background-color: lightgray; font-size:18px");
    linkInput->setAcceptRichText(false);
    layout->addWidget(linkInput,0,Qt::AlignCenter);

    QPushButton *getLinkButton = new QPushButton("Get Link");
    getLinkButton->setFixedSize(150,50);
    getLinkButton->setStyleSheet("background-color: blue; color: white; font-size:18px");
    layout->addWidget(getLinkButton,0,Qt::AlignCenter);

    videoInfoLabel = new QLabel("");
    videoInfoLabel->setAlignment(Qt::AlignCenter);
    videoInfoLabel->setStyleSheet("font-size:18px; color: green");
    layout->addWidget(videoInfoLabel);


    downloadButton = new QPushButton("Download");
    downloadButton->setFixedSize(150,50);
    downloadButton->setStyleSheet("background-color: red; color: white; font-size:20px");
    downloadButton->hide();
    layout->addWidget(downloadButton,0,Qt::AlignCenter);

    manager = new QNetworkAccessManager();

    QObject::connect(getLinkButton, &QPushButton::clicked,[&](){
        QString videoLink = linkInput->toPlainText();
        fetchVideoInfo(videoLink,&mainWindow);
    });

    QObject::connect(downloadButton, &QPushButton::clicked,[&](){
        QString videoLink = linkInput->toPlainText();
        QWidget *downloadWindow = new QWidget();
        downloadWindow->setWindowTitle("Downloading..");
        downloadWindow->resize(400,200);

        QVBoxLayout *downloadLayout = new QVBoxLayout(downloadWindow);

        QLabel *downloadingLabel = new QLabel("Downloading.. ");
        downloadingLabel->setAlignment(Qt::AlignCenter);
        downloadingLabel->setStyleSheet("font-size:16px; color:red");

        downloadLayout->addWidget(downloadingLabel);

        progressLabel = new QLabel("");
        progressLabel->setAlignment(Qt::AlignCenter);
        progressLabel->setStyleSheet("font-size:18px; color: green");
        downloadLayout->addWidget(progressLabel);

        downloadWindow->show();

        downloadVideo(videoLink,downloadWindow);
    });






    mainWindow.show();

    return a.exec();


    /*
    MainWindow w;
    w.show();
    return a.exec();
    */
}
