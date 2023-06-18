#include "dataaccessorbase.h"

#include <QSqlError>


DataAccessorBase::DataAccessorBase(DatabaseConnection* connection, QObject *parent): QObject{parent}
{
    m_connection = connection;
}

void DataAccessorBase::getData(QSqlQueryModel* model)
{
    const auto sql = buildSql();
    if (m_connection->executeQuery(sql, model))
    {
        initHeaders(model);
    }

    auto lastError = model->lastError();

    if (lastError.type() == QSqlError::NoError)
    {
        emit sig_DataReady(model);
    }
    else
    {
        emit sig_QueryError(lastError.text());
    }
}
