#include "daystatisticsdataaccessor.h"

DayStatisticsDataAccessor::DayStatisticsDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

const QString &DayStatisticsDataAccessor::getAirportCode() const
{
    return m_airPortCode;
}

void DayStatisticsDataAccessor::setAirportCode(const QString &airportCode)
{
    m_airPortCode = airportCode;
}

void DayStatisticsDataAccessor::setAirportCode(QString &&airportCode)
{
    m_airPortCode = airportCode;
}

void DayStatisticsDataAccessor::buildSql(QSqlQuery &query) const
{
    query.prepare("SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                  "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = :airportCode or arrival_airport = :airportCode) "
                  "GROUP BY \"Day\"");

    query.bindValue(":airportCode", m_airPortCode);
}

void DayStatisticsDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Количество рейсов"));
    model->setHeaderData(1, Qt::Horizontal, tr("День"));
}
