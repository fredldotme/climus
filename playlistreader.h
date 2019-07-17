#ifndef PLAYLISTREADER_H
#define PLAYLISTREADER_H

#include <QObject>
#include <QString>
#include <QVector>

class PlaylistReader : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistReader(QObject *parent = nullptr);

signals:
    void readError(const QString reason);
    void playlistRead(const QVector<QString> musicList);

public slots:
};

#endif // PLAYLISTREADER_H
