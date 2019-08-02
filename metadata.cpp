#include <QFile>
#include <QDir>
#include <QDebug>

#include "metadata.h"

#include "taglib/taglib.h"
#include <taglib/fileref.h>
#include <taglib/tfile.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

metadata::metadata()
{

}

void metadata::run()
{
    isRunning = 1;
    loadDirectory(dirPath);
}

void metadata::setUpDirPath(QString dirPathStr){
    dirPath = dirPathStr;
}

void metadata::loadDirectory(QString dirPath){
    QDir dir(dirPath);

    foreach(QFileInfo fileInfo , dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries)){
        if(isRunning == 1){
            QFile file(fileInfo.filePath());
    //      file.copy("/tmp/songs_location/"+fileInfo.fileName()+".mp3");
            loadMetaData(file.fileName());
            file.deleteLater();
            msleep(100);
        }
    }
    stopRunning();
}

void metadata::stopRunning()
{
    isRunning = 0;
}

void metadata::loadMetaData(QString file){
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
                            emit signalMetaLoaded(metaList,pixMap);
                        }
                    }
                }else{
                    QPixmap pixMap(":/data/noCover.png");
                    emit signalMetaLoaded(metaList,pixMap);
                }
            }
    }

}
