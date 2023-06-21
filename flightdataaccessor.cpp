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
        query.prepare("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as airportName from bookings.flights f "
                      "JOIN bookings.airports_data ad ON ad.airport_code = f.departure_airport "
                      "WHERE f.arrival_airport = :airportCode "
                      "AND f.scheduled_arrival >= :date AND f.scheduled_arrival < (:date::timestamptz + interval '1 day') "
                      "ORDER BY scheduled_arrival");
    }
    else
    {
        query.prepare("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as airportName from bookings.flights f "
                      "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
                      "where f.departure_airport = :airportCode "
                      "AND f.scheduled_departure >= :date AND f.scheduled_departure < (:date::timestamptz + interval '1 day') "
                      "ORDER BY scheduled_departure");
    }

    query.bindValue(":airportCode", m_airPortCode);
    query.bindValue(":date", m_date);
}

void FlightDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    model->setHeaderData(1, Qt::Horizontal, tr(m_direction == FlightDirection::Arrival ? "Время прилета" : "Время вылета"));
    model->setHeaderData(2, Qt::Horizontal, tr(m_direction == FlightDirection::Arrival ? "Аэропорт отправления" : "Аэропорт назначения"));
}
