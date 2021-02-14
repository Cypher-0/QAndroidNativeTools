#include <qglobal.h>

#ifdef Q_OS_ANDROID

#ifndef ANDROID_GALLERY_RESULT_RECEIVER_H
#define ANDROID_GALLERY_RESULT_RECEIVER_H

#include <QAndroidActivityResultReceiver>
#include <QObject>
#include <QStringList>

namespace android
{

class GalleryResultReceiver  final : public QObject,public QAndroidActivityResultReceiver
{
Q_OBJECT

public:
    GalleryResultReceiver() : QObject(),QAndroidActivityResultReceiver()
    {

    }

    ~GalleryResultReceiver()
    {

    }

signals:
    void filesSelected(QStringList list);

public:
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) override;
};

bool askImages(GalleryResultReceiver* receiver);

}

#endif // CANDROIDACTIVITYRESULTRECEIVER_H

#endif //#ifdef Q_OS_ANDROID
