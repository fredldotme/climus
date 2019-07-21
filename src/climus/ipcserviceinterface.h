#ifndef IPCSERVICEINTERFACE_H
#define IPCSERVICEINTERFACE_H

#include <QObject>
#include <commonconsts.h>

class IPCServiceInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", CLIMUSD_SERVICE_INTERFACE)
public:
    explicit IPCServiceInterface(QObject *parent = nullptr);

signals:
    void playlistReadRequest(const QString& filePath);

public slots:
    void readPlaylist(const QString& filePath);
};

#endif // IPCSERVICEINTERFACE_H
