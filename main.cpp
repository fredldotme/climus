#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <iostream>

#include "musicplayer.h"
#include "playlistreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Music player objects
    PlaylistReader playlistReader;
    MusicPlayer musicPlayer;

    // Command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription("Simple m3u music player");
    QCommandLineOption playlistFileOption(QStringList() << "p" << "playlist",
                                          "Read m3u playlist from <file>.",
                                          "file");
    parser.addOption(playlistFileOption);
    parser.process(app);

    const bool playlistGiven = parser.isSet(playlistFileOption);
    if (!playlistGiven) {
        std::cerr << "No playlist (argument -p <file>) given, bailing out..." << std::endl;
        exit(1);
    }

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
                     &app, [=](const float progressPercent) {
        const int progressBarMaxWidth = 20;
        const int progressBarEnd = static_cast<int>(progressBarMaxWidth *
                                                    progressPercent);

        std::cout << "[";
        for (int i = 0; i < progressBarEnd; i++) {
            std::cout << "#";
        }
        for (int i = 0; i < progressBarMaxWidth - progressBarEnd; i++) {
            std::cout << " ";
        }
        std::cout << "]";
        std::cout << "\r";
        std::cout.flush();
    });

    // Start reading the playlist
    playlistReader.readPlaylist(parser.value(playlistFileOption));

    return app.exec();
}
