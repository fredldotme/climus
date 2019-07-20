#include "ipcserviceinterface.h"

IPCServiceInterface::IPCServiceInterface(QObject *parent) : QObject(parent)
{

}

void IPCServiceInterface::readPlaylist(const QString& filePath)
{
    emit playlistReadRequest(filePath);
}
