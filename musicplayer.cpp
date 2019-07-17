#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent)
{
    this->m_internalPlayer.setPlaylist(&this->m_playlist);

    QObject::connect(&this->m_internalPlayer, &QMediaPlayer::stateChanged,
                     this, [=](QMediaPlayer::State state) {

        const QString filePath =
                this->m_playlist.currentMedia().canonicalUrl().toLocalFile();

        switch (state) {
        case QMediaPlayer::State::PlayingState:
            emit musicStarted(filePath);
            break;
        case QMediaPlayer::State::StoppedState:
            emit musicStopped(filePath);
            break;
        default:
            break;
        }
    });
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
