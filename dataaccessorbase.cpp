#include "dataaccessorbase.h"

#include <QSqlError>


DataAccessorBase::DataAccessorBase(DatabaseConnection* connection, QObject *parent): QObject{parent}
{
    m_connection = connection;
}

bool DataAccessorBase::getData(QSqlQueryModel* model)
{
    const auto sql = buildSql();
    if (m_connection->executeQuery(sql, model))
    {
        initHeaders(model);
        return model->lastError().type() == QSqlError::NoError;
    }

    return false;
}
