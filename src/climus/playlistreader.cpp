#include "playlistreader.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

PlaylistReader::PlaylistReader(QObject *parent) : QObject(parent)
{

}

void PlaylistReader::readPlaylist(const QString& playlist)
{
    QFile playlistFile(QUrl::fromLocalFile(playlist).path());

    if (!playlistFile.exists()) {
        emit readError(QStringLiteral("File %1 doesn't exist").arg(playlistFile.fileName()));
        return;
    }

    if (!playlistFile.open(QFile::ReadOnly)) {
        emit readError(QStringLiteral("Failed to open playlist file"));
        return;
    }

    // m3u files contain newline-separated paths to the respective music files.
    const QByteArray content = playlistFile.readAll();
    playlistFile.close();

    // Only support \n for now
    if (content.contains('\r')) {
        emit readError(QStringLiteral("Playlist contains \\r, please convert using dos2unix"));
        return;
    }

    const QList<QByteArray> lines = content.split('\n');
    QQueue<QUrl> playlistQueue;

    for (const QByteArray& line : lines) {
        // Skip empty lines
        if (line.trimmed().isEmpty()) {
            continue;
        }

        // Support music files relative to their pertaining playlist
        const QString musicFile = QFileInfo(playlistFile).absoluteDir().absolutePath() +
                QDir::separator() + QString::fromUtf8(line);

        const bool musicFileExists = QFile::exists(musicFile);
        if (!musicFileExists) {
            emit readError(QStringLiteral("Music file '%1' doesn't exist").arg(musicFile));
            return;
        }

        playlistQueue.enqueue(QUrl::fromLocalFile(musicFile));
    }

    emit playlistRead(playlistQueue);
}
