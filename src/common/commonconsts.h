#ifndef DAEMONCONSTS_H
#define DAEMONCONSTS_H

#include <QString>

namespace Climus {
enum ExitCodes {
    EXIT_FAILURE_NO_ARGUMENT = 1,
    EXIT_FAILURE_SERVICE_REGISTRATION,
    EXIT_FAILURE_OBJECT_REGISTRATION,
    EXIT_FAILURE_PLAYLIST_READ_ERROR
};
}

#define CLIMUSD_SERVICE_INTERFACE "me.fredl.climusd.Control"
const QString CLIMUSD_SERVICE_NAME = QStringLiteral("me.fredl.climusd");
const QString CLIMUSD_SERVICE_PATH = QStringLiteral("/");

#endif // DAEMONCONSTS_H
