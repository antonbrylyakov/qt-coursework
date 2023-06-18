#include "databaseconnection.h"
#include <QSqlError>


DatabaseConnection::DatabaseConnection(QObject *parent)
    : QObject{parent}
{
    m_database =  QSqlDatabase::addDatabase("QPSQL");
    m_reconnectTimer = new QTimer(this);
    connect(m_reconnectTimer, &QTimer::timeout, [this](){ doOpen(true); });
}

DatabaseConnection::DatabaseConnection(DatabaseConfiguration& config, QObject *parent):DatabaseConnection{parent}
{
    m_configuration = config;
}

DatabaseConnection::~DatabaseConnection()
{
    m_reconnectTimer->stop();
    close();
}

void DatabaseConnection::setConfiguration(DatabaseConfiguration config)
{
    m_configuration = config;
    configure();
}

void DatabaseConnection::open(bool untilSuccess)
{
    if (m_status == DatabaseConnectionStatus::Disconnected)
    {
        setStatus(DatabaseConnectionStatus::Connecting);
        doOpen(untilSuccess);
    }
}

void DatabaseConnection::close()
{
    m_database.close();
    setStatus(DatabaseConnectionStatus::Disconnected);
}

bool DatabaseConnection::executeQuery(QString query, QSqlQueryModel *toModel)
{
    toModel->setQuery(query, m_database);
    if (toModel->lastError().type() != QSqlError::NoError)
    {
        if (m_configuration.disconnectOnError)
        {
            close();
        }

        return false;
    }

    return true;
}

void DatabaseConnection::doOpen(bool untilSuccess)
{
    if (m_status != DatabaseConnectionStatus::Connecting)
    {
        return;
    }

    m_reconnectTimer->stop();
    configure();
    auto connected = m_database.open();
    if (connected)
    {
        setStatus(DatabaseConnectionStatus::Connected);
    }
    else if (untilSuccess)
    {
        m_reconnectTimer->start();
    }
    else
    {
        setStatus(DatabaseConnectionStatus::Disconnected);
        emit sig_ConnectionError(m_database.lastError().text());
    }
}

void DatabaseConnection::configure()
{
    close();
    m_reconnectTimer->setInterval(m_configuration.reconnectIntervalSeconds * 1000);
    m_database.setHostName(m_configuration.host);
    m_database.setPort(m_configuration.port);
    m_database.setDatabaseName(m_configuration.dbName);
    m_database.setUserName(m_configuration.userName);
    m_database.setPassword(m_configuration.password);
}

void DatabaseConnection::setStatus(DatabaseConnectionStatus status)
{
    bool changed = status != m_status;
    m_status = status;
    if (changed)
    {
        emit sig_ChangeConnectionStatus(m_status);
    }
}
