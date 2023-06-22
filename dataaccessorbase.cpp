#include "dataaccessorbase.h"

#include <QSqlError>


DataAccessorBase::DataAccessorBase(DatabaseConnection* connection, QObject *parent): QObject{parent}
{
    m_connection = connection;
}

void DataAccessorBase::getData(QSqlQueryModel* model)
{
    auto query = m_connection->createQuery();
    buildSql(query);
    query.exec();

    if (m_connection->executeQuery(std::move(query), model))
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
