#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusReply>

#include "daemonconsts.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc != 2) {
        std::cerr << "Exactly one argument expected, bailing out..." << std::endl;
        exit(1);
    }

    const char* filePath = argv[1];
    QDBusInterface serviceInterface(CLIMUSD_SERVICE_NAME,
                                    CLIMUSD_SERVICE_PATH,
                                    CLIMUSD_SERVICE_INTERFACE);

    QDBusReply<void> reply =
            serviceInterface.call(QStringLiteral("readPlaylist"),
                                  QString::fromUtf8(filePath, static_cast<int>(strlen(filePath))));

    if (!reply.isValid()) {
        std::cerr << "Reply is invalid, reason: "
                  << reply.error().message().toStdString() << std::endl;
        exit(2);
    }

    return 0;
}
