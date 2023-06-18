#ifndef DATAACCESSORBASE_H
#define DATAACCESSORBASE_H

#include <QObject>
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
    virtual QString buildSql() = 0;
    virtual void initHeaders(QSqlQueryModel* model) = 0;

signals:
    void sig_DataReady(QSqlQueryModel* model);
    void sig_QueryError(QString msg);
};

#endif // DATAACCESSORBASE_H
