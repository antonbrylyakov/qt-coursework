#include "yearstatisticsdataaccessor.h"

YearStatisticsDataAccessor::YearStatisticsDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

const QString &YearStatisticsDataAccessor::getAirportCode() const
{
    return m_airPortCode;
}

void YearStatisticsDataAccessor::setAirportCode(const QString &airportCode)
{
    m_airPortCode = airportCode;
}

void YearStatisticsDataAccessor::setAirportCode(QString &&airportCode)
{
    m_airPortCode = airportCode;
}

void YearStatisticsDataAccessor::buildSql(QSqlQuery &query) const
{
    query.prepare("SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                  "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = :airportCode or arrival_airport = :airportCode ) "
                  "group by \"Month\"");

    query.bindValue(":airportCode", m_airPortCode);
}

void YearStatisticsDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Количество рейсов"));
    model->setHeaderData(1, Qt::Horizontal, tr("Месяц"));
}


