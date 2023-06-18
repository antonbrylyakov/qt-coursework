#ifndef DATABASECONFIGURATION_H
#define DATABASECONFIGURATION_H

#endif // DATABASECONFIGURATION_H

#include <QString>


struct DatabaseConfiguration
{
    QString host;
    int port;
    QString dbName;
    QString userName;
    QString password;
    bool disconnectOnError = true;
    int reconnectIntervalSeconds = 5;
};
