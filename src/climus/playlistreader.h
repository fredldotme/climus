#ifndef PLAYLISTREADER_H
#define PLAYLISTREADER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QQueue>

class PlaylistReader : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistReader(QObject *parent = nullptr);

public slots:
    void readPlaylist(const QString& playlist);

signals:
    void readError(const QString reason);
    void playlistRead(QQueue<QUrl> musicList);
};

#endif // PLAYLISTREADER_H
