#include "flightdataaccessor.h"

#include <QSqlQuery>

FlightDataAccessor::FlightDataAccessor(DatabaseConnection *connection, QObject *parent):DataAccessorBase{connection, parent}
{

}

const QString &FlightDataAccessor::getAirportCode() const
{
    return m_airPortCode;
}

void FlightDataAccessor::setAirportCode(const QString &airportCode)
{
    m_airPortCode = airportCode;
}

void FlightDataAccessor::setAirportCode(QString &&airportCode)
{
    m_airPortCode = airportCode;
}

const QDate &FlightDataAccessor::getDate() const
{
    return m_date;
}

void FlightDataAccessor::setDate(const QDate &date)
{
    m_date = date;
}

void FlightDataAccessor::setDate(QDate &&date)
{
    m_date = date;
}

const FlightDirection FlightDataAccessor::getFlightDirection() const
{
    return m_direction;
}

void FlightDataAccessor::setFlightDirection(const FlightDirection direction)
{
    m_direction = direction;
}

void FlightDataAccessor::buildSql(QSqlQuery& query) const
{
    if (m_direction == FlightDirection::Arrival)
    {
        query.prepare("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"airportName\" from bookings.flights f"
                              "JOIN bookings.airport_data ad ON ad.airport_code = :airportCode"
                              "where f.arrival_airport = :airportCode"
                              "AND f.scheduled_arrival >= :date AND f.scheduled_arrival < (:date + interval('1 day')");
    }
    else
    {
        query.prepare("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"airportName\" from bookings.flights f"
                              "JOIN bookings.airport_data ad ON ad.airport_code = :airportCode"
                              "where f.departure_airport = :airportCode"
                              "AND f.scheduled_departure >= :date AND f.scheduled_departure < (:date + interval('1 day')");
    }

    query.bindValue(":airportCode", m_airPortCode);
    query.bindValue(":date", m_date);
}

void FlightDataAccessor::initHeaders(QSqlQueryModel *model) const
{

}
