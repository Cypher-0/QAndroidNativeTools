#include <qglobal.h>

#ifdef Q_OS_ANDROID

#ifndef ANDROID_UTILS_H
#define ANDROID_UTILS_H

#include <QStringList>

namespace android
{

QStringList requirePermissions(const QStringList& permissions);

}//namespace android


#endif // ANDROID_UTILS_H

#endif //Q_OS_ANDROID
