#ifndef DATAACCESSORBASE_H
#define DATAACCESSORBASE_H

#include <QObject>
#include <QSqlQuery>
#include "databaseconnection.h"

// Базовый класс для доступа к данным
class DataAccessorBase: public QObject
{
    Q_OBJECT
public:
    // Объект не владеет подключением
    DataAccessorBase(DatabaseConnection* connection, QObject *parent = nullptr);
    virtual void getData(QSqlQueryModel* model);
protected:
    DatabaseConnection* m_connection = nullptr;
    virtual void buildSql(QSqlQuery& query) const = 0;
    virtual void initHeaders(QSqlQueryModel* model) const = 0;

signals:
    void sig_DataReady(QSqlQueryModel* model);
    void sig_QueryError(QString msg);
};

#endif // DATAACCESSORBASE_H
