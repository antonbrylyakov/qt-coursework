#include "airportdataaccessor.h"

AirportDataAccessor::AirportDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

void AirportDataAccessor::buildSql(QSqlQuery& query) const
{
    query.prepare(QUERY_TEXT);
}

void AirportDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Название"));
    model->setHeaderData(1, Qt::Horizontal, tr("Код"));
}
