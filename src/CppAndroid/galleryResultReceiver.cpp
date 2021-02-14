#include "galleryResultReceiver.h"

#ifdef Q_OS_ANDROID

#include <QDebug>

#include <QtAndroidExtras>

#include <QStringList>

namespace android
{

void GalleryResultReceiver::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    auto abortLambda{
        [&](const QString& msg)
        {
            qCritical() << "Aborting " << __func__ << " : " << msg;
            throw std::runtime_error(msg.toStdString());
        }
    };

    //qDebug() << "-------------------- " << __func__ << " ------------------------";

    //qDebug() << "Receiver request code :" << receiverRequestCode;
    //qDebug() << "Result code :" << resultCode;
    //qDebug() << data.toString();

    QStringList output{};

    char java_galleryUtils[] = "QAndroidNativeTools/GalleryOpener";

    if(!QAndroidJniObject::isClassAvailable(java_galleryUtils))
    {
        abortLambda("Unavailable java class : "+QString(java_galleryUtils));
    }

    auto javReturn{QAndroidJniObject::callStaticObjectMethod( //check this out : https://doc.qt.io/archives/qt-5.9/qandroidjniobject.html
                    java_galleryUtils,
                    "onActivityResult",
                    "(IILandroid/content/Intent;Landroid/content/Context;)[Ljava/lang/String;",//function taking 4 args : int,int,Intent,Context and returning string
                    receiverRequestCode,resultCode,data.object(),QtAndroid::androidContext().object())};

    if(!javReturn.isValid())
    {
        abortLambda("Invalid selected files list");
    }

    QAndroidJniEnvironment jniEnv;
    const jobjectArray stringListObjArray = javReturn.object<jobjectArray>();
    const int arraySize= jniEnv->GetArrayLength(stringListObjArray );

    for(int idx = 0; idx < arraySize; ++idx)
    {
        const QAndroidJniObject stringObj = QAndroidJniObject::fromLocalRef(jniEnv->GetObjectArrayElement(stringListObjArray , idx));
        output << stringObj .toString();
    }


    emit filesSelected(output);
}

bool askImages(GalleryResultReceiver* receiver)
{
    QAndroidJniObject action = QAndroidJniObject::fromString( "android.intent.action.PICK");
    QAndroidJniObject intent( "android/content/Intent");
    QAndroidJniObject Intent__EXTRA_ALLOW_MULTIPLE = QAndroidJniObject::getStaticObjectField("android/content/Intent", "EXTRA_ALLOW_MULTIPLE", "Ljava/lang/String;");

    if (!action.isValid() || !intent.isValid() || !Intent__EXTRA_ALLOW_MULTIPLE.isValid()) {
        qDebug() << "Activity start error";
        return false;
    }
    else
    {
        qDebug() << "Successfully opened gallery";
    }

    intent.callObjectMethod( "setAction", "(Ljava/lang/String;)Landroid/content/Intent;", action.object<jstring>());
    intent.callObjectMethod( "setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString("image/*").object<jstring>());
    intent.callObjectMethod("putExtra","(Ljava/lang/String;Z)Landroid/content/Intent;", Intent__EXTRA_ALLOW_MULTIPLE.object<jobject>(), true);
    QtAndroid::startActivity(intent.object<jobject>(), 1,receiver);
    qDebug() << "STARTED";

    return true;
}

}//namespace android


#endif //Q_OS_ANDROID
