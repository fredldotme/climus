#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QQueue>
#include <QString>
#include <QUrl>

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);

private:
    const QString getCurrentFile();

    QMediaPlaylist m_playlist;
    QMediaPlayer m_internalPlayer;

signals:
    void musicStarted(const QString musicFile);
    void progressChanged(const float progressPercent);

public slots:
    void enqueueNewPlaylist(QQueue<QUrl> musicList);
};

#endif // MUSICPLAYER_H
