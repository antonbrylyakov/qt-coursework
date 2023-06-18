#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QTimer>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databaseconnectionstatus.h"
#include "databaseconfiguration.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConnection(QObject *parent = nullptr);
    DatabaseConnection(DatabaseConfiguration& config, QObject* parent = nullptr);
    ~DatabaseConnection();
    void setConfiguration(DatabaseConfiguration config);
    void open(bool untilSuccess = false);
    void close();
    QSqlQuery createQuery() const;
    bool executeQuery(QSqlQuery&& query, QSqlQueryModel* toModel);
signals:
    void sig_ChangeConnectionStatus(DatabaseConnectionStatus status);
    void sig_ConnectionError(QString msg);
private:
    DatabaseConnectionStatus m_status = DatabaseConnectionStatus::Disconnected;
    DatabaseConfiguration m_configuration;
    QSqlDatabase m_database;
    QTimer* m_reconnectTimer;
    void doOpen(bool untilSuccess);
    void configure();
    void setStatus(DatabaseConnectionStatus status);
};

#endif // DATABASECONNECTION_H
