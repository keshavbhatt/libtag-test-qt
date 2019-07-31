#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDir>
#include <QDebug>


#include "taglib/taglib.h"
#include <taglib/fileref.h>
#include <taglib/tfile.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMetaData(QString file) {
    QByteArray fileName = QFile::encodeName( file );
    const char * encodedName = fileName.constData();
    TagLib::FileRef fileref = TagLib::FileRef( encodedName );
    if (fileref.isNull())
    {
        qDebug()<<"is Null";
    }
    else
    {
        QStringList metaList;
        metaList.append("<b>FileName:</b> " +file);
        metaList.append("<b>Length:</b> " +QString::number(fileref.audioProperties()->length()));
        metaList.append("<b>SampleRate:</b> "+QString::number(fileref.audioProperties()->sampleRate()));
        metaList.append("<b>Bitrate:</b> "+QString::number(fileref.audioProperties()->bitrate()));
        metaList.append("<b>Title:</b> " +QString(fileref.tag()->title().toCString()));
        metaList.append("<b>Artist:</b> "+QString(fileref.tag()->artist().toCString()));
        metaList.append("<b>Album:</b> "+QString(fileref.tag()->album().toCString()));
        metaList.append("<b>Genre:</b> "+QString(fileref.tag()->genre().toCString()));
        metaList.append("<b>Year:</b> "+QString::number(fileref.tag()->year()));
        metaList.append("<b>Comment:</b> "+QString(fileref.tag()->comment().toCString()).split(" ").first());

        QWidget *itemWidget = new QWidget(ui->listWidget);
        listItem_Ui.setupUi(itemWidget);

        foreach(QString info,metaList){
            QLabel *item = new QLabel(info);
            listItem_Ui.meta->addWidget(item);
        }

        TagLib::MPEG::File mpegFile(fileref.file()->name());
            if (mpegFile.isValid() == true || mpegFile.ID3v2Tag() != nullptr) {
                TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
                const TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");
                if (!frameList.isEmpty()) {
                    TagLib::ID3v2::AttachedPictureFrame* picFrame;
                    picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
                    if (picFrame != nullptr) {
                        QString cover = picFrame->mimeType().toCString();
                        if(!cover.isEmpty()){
                            QPixmap pixMap;
                            pixMap.loadFromData(reinterpret_cast<const uchar *>(picFrame->picture().data()),picFrame->picture().size());
                            listItem_Ui.cover->setPixmap(pixMap.scaled(listItem_Ui.cover->size(),Qt::KeepAspectRatio));
                        }
                    }
                }else{
                    QPixmap pixMap(":/data/noCover.png");
                    listItem_Ui.cover->setPixmap(pixMap.scaled(listItem_Ui.cover->size(),Qt::KeepAspectRatio));
                }
            }

            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            item->setSizeHint(itemWidget->minimumSizeHint());

            ui->listWidget->setItemWidget(item,itemWidget);
            ui->listWidget->addItem(item);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QDir dir("/tmp/songs_location");
    qDebug()<<dir.entryList(QDir::NoDotAndDotDot|QDir::AllEntries).count();

    foreach(QFileInfo fileInfo , dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries)){
        QFile file(fileInfo.filePath());
//      file.copy("/tmp/songs_location/"+fileInfo.fileName()+".mp3");
        loadMetaData(file.fileName());
        file.deleteLater();
    }
    qDebug()<<ui->listWidget->count();
}

void MainWindow::on_clear_clicked()
{
    ui->listWidget->clear();
}
