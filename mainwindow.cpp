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

void MainWindow::on_pushButton_clicked()
{
    meta_thread.disconnect();
    meta_thread.setUpDirPath("/tmp/songs_location");
    QObject::connect(&meta_thread,SIGNAL(signalMetaLoaded(const QStringList&,const QPixmap&)),this,SLOT(setMetaData(const QStringList&, const QPixmap&)), Qt::QueuedConnection);
    meta_thread.start();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
}

void MainWindow::setMetaData(const QStringList metaList ,const QPixmap pix){

    tasksProgress++;
    auto val = 100*tasksProgress/QDir("/tmp/songs_location").entryList(QDir::NoDotAndDotDot|QDir::AllEntries).count();
    ui->progressBar->setValue(val);

    ui->lineEdit->setText("Processing: "+QString(metaList.at(0)).split(":</b> ").last());

    QWidget *itemWidget = new QWidget(ui->listWidget);
    listItem_Ui.setupUi(itemWidget);

    foreach(QString info,metaList){
        QLabel *item = new QLabel(info);
        listItem_Ui.meta->addWidget(item);
    }

    listItem_Ui.cover->setPixmap(pix.scaled(listItem_Ui.cover->size(),Qt::KeepAspectRatio));

    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(itemWidget->minimumSizeHint());

    ui->listWidget->setItemWidget(item,itemWidget);
    ui->listWidget->addItem(item);

}

void MainWindow::on_clear_clicked()
{
    while(ui->listWidget->count()>0){
        delete(ui->listWidget->takeItem(ui->listWidget->count()-1));
    }
    ui->progressBar->setValue(0);
    tasksProgress = 0;
}

void MainWindow::on_stopThread_clicked()
{
    meta_thread.stopRunning();
    ui->progressBar->setValue(0);
    tasksProgress = 0;
}
