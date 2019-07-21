#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent)
{
    this->m_internalPlayer.setPlaylist(&this->m_playlist);

    QObject::connect(&this->m_playlist, &QMediaPlaylist::currentMediaChanged,
                     this, [=](){
        const QString currentFile = getCurrentFile();
        if (currentFile.isEmpty())
            return;

        emit musicStarted(currentFile);
    });

    QObject::connect(&this->m_internalPlayer, &QMediaPlayer::positionChanged,
                     this, [=](qint64 position) {
        if (this->m_internalPlayer.state() != QMediaPlayer::PlayingState) {
            return;
        }
        const qint64 duration = this->m_internalPlayer.duration();
        const float progressPercent = (static_cast<float>(position) /
                                       static_cast<float>(duration));
        emit progressChanged(progressPercent);
    });
}

const QString MusicPlayer::getCurrentFile()
{
    const QString filePath =
            this->m_playlist.currentMedia().canonicalUrl().toLocalFile();
    return filePath;
}

void MusicPlayer::enqueueNewPlaylist(QQueue<QUrl> musicList)
{
    this->m_internalPlayer.stop();
    this->m_playlist.clear();

    for (const QUrl& file : musicList) {
        const QMediaContent media(file);
        this->m_playlist.addMedia(media);
    }

    this->m_playlist.setCurrentIndex(0);
    this->m_internalPlayer.play();
}
