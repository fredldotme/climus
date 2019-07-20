#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDBusConnection>

#include <iostream>

#include <daemonconsts.h>
#include "ipcserviceinterface.h"
#include "musicplayer.h"
#include "playlistreader.h"
#include "progressoutput.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Music player objects
    PlaylistReader playlistReader;
    MusicPlayer musicPlayer;
    ProgressOutput progressOutput;

    // Command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription("Simple m3u music player");
    QCommandLineOption playlistFileOption(QStringList() << "p" << "playlist",
                                          "Read m3u playlist from <file>.",
                                          "file");
    QCommandLineOption daemonOption(QStringList() << "d" << "daemon");
    parser.addOption(playlistFileOption);
    parser.addOption(daemonOption);
    parser.process(app);

    const bool playlistGiven = parser.isSet(playlistFileOption);
    const bool daemonMode = parser.isSet(daemonOption);

    // Connect signals to slots (loose coupling)
    // Music playlist reading
    QObject::connect(&playlistReader, &PlaylistReader::readError,
                     &app, [=](const QString reason) {
        std::cerr << "Playlist read error: " << reason.toStdString() << std::endl;
        exit(2);
    });
    QObject::connect(&playlistReader, &PlaylistReader::playlistRead,
                     &musicPlayer, &MusicPlayer::enqueueNewPlaylist);

    // Informational output
    QObject::connect(&musicPlayer, &MusicPlayer::musicStarted,
                     &app, [=](const QString fileName){
        std::cout << "Music started: " << fileName.toStdString() << std::endl;
    });
    QObject::connect(&musicPlayer, &MusicPlayer::progressChanged,
                     &progressOutput, &ProgressOutput::handleProgressChange);

    if (playlistGiven) {
        // Start reading the playlist
        playlistReader.readPlaylist(parser.value(playlistFileOption));

    } else if (daemonMode) {
        const bool serviceUp = QDBusConnection::sessionBus().registerService(CLIMUSD_SERVICE_NAME);
        if (!serviceUp) {
            std::cerr << "Service " << CLIMUSD_SERVICE_NAME.toStdString() <<
                         " couldn't be registered, exiting" << std::endl;
            exit(1);
        }

        // The object exposed via the DBus IPC mechanism
        IPCServiceInterface* serviceInterface = new IPCServiceInterface(&app);

        const bool objectRegistered = QDBusConnection::sessionBus().registerObject(CLIMUSD_SERVICE_PATH,
                                                                                   serviceInterface,
                                                                                   QDBusConnection::ExportAllSlots);
        if (!objectRegistered) {
            std::cerr << "Failed to register object at path " <<
                         CLIMUSD_SERVICE_PATH.toStdString() << std::endl;
            exit(2);
        }

        // As the object is registered now, connect serviceInterface with the playlistReader
        QObject::connect(serviceInterface, &IPCServiceInterface::playlistReadRequest,
                         &playlistReader, &PlaylistReader::readPlaylist);
    } else {
        std::cerr << "Neither -p <file> nor -d specified, bailing out" << std::endl;
        exit(3);
    }

    return app.exec();
}
