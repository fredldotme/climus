#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QString>
#include <QVector>

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);

signals:
    void musicStarted(const QString musicFile);
    void musicStopped(const QString musicFile);

public slots:
    void enqueueNewPlaylist(const QVector<QString> musicList);
};

#endif // MUSICPLAYER_H
