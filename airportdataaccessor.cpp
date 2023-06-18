#include "airportdataaccessor.h"

AirportDataAccessor::AirportDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

QString AirportDataAccessor::buildSql()
{
    return QString("SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data ORDER BY airport_name->>'ru'");
}

void AirportDataAccessor::initHeaders(QSqlQueryModel *model)
{
    model->setHeaderData(0, Qt::Horizontal, tr("Название"));
    model->setHeaderData(1, Qt::Horizontal, tr("Код"));
}
