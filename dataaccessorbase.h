#ifndef DATAACCESSORBASE_H
#define DATAACCESSORBASE_H

#include <QObject>
#include "databaseconnection.h"

// Базовый класс для доступа к данным
class DataAccessorBase: QObject
{
    Q_OBJECT
public:
    // Объект не владеет подключением
    DataAccessorBase(DatabaseConnection* connection, QObject *parent = nullptr);
    virtual bool getData(QSqlQueryModel* model);
protected:
    DatabaseConnection* m_connection = nullptr;
    virtual QString buildSql() = 0;
    virtual void initHeaders(QSqlQueryModel* model) = 0;
};

#endif // DATAACCESSORBASE_H
