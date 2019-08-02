#ifndef METADATA_H
#define METADATA_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QPixmap>


class metadata : public QThread
{
    Q_OBJECT
public slots:
    void stopRunning();
    void loadMetaData(QString file);
    void loadDirectory(QString dirPath);


    void setUpDirPath(QString dirPathStr);
protected:
   virtual void run();

signals:
   void signalMetaLoaded(QStringList meta,QPixmap pix);

private:
    bool isRunning;
    QString dirPath;

public:
    metadata();
};

#endif // METADATA_H
