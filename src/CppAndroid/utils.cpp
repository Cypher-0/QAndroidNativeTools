#include "utils.h"

#ifdef Q_OS_ANDROID

#include <QtAndroid>

namespace android
{

QStringList requirePermissions(const QStringList& permissions)
{
    QStringList refusedPerm{};

    //Request requiered permissions at runtime
    for(const QString &permission : permissions)
    {
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied)
        {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
            {
                refusedPerm << permission;
            }
        }
    }
    return refusedPerm;
}


}//namespace android

#endif //Q_OS_ANDROID
