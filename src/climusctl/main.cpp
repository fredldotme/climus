#include <QCoreApplication>
#include <QDBusInterface>

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

    serviceInterface.call(QStringLiteral("readPlaylist"),
                          QString::fromUtf8(filePath, strlen(filePath)));
    return 0;
}
