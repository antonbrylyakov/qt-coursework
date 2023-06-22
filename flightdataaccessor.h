#ifndef FLIGHTDATAACCESSOR_H
#define FLIGHTDATAACCESSOR_H

#include "dataaccessorbase.h"

#include <QDate>

enum class FlightDirection
{
    Departure,
    Arrival
};

class FlightDataAccessor : public DataAccessorBase
{
public:
    FlightDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
    const QString& getAirportCode() const;
    void setAirportCode(const QString& airportCode);
    void setAirportCode(QString&& airportCode);
    const QDate& getDate() const;
    void setDate(const QDate& date);
    void setDate(QDate&& date);
    const FlightDirection getFlightDirection() const;
    void setFlightDirection(const FlightDirection direction);
protected:
    inline static const QString QUERY_TEXT_DEPARTURE = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as airportName from bookings.flights f "
                                                       "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
                                                       "where f.departure_airport = :airportCode "
                                                       "AND f.scheduled_departure >= :date AND f.scheduled_departure < (:date::timestamptz + interval '1 day') "
                                                       "ORDER BY scheduled_departure";

    inline static const QString QUERY_TEXT_ARRIVAL = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as airportName from bookings.flights f "
                                                     "JOIN bookings.airports_data ad ON ad.airport_code = f.departure_airport "
                                                     "WHERE f.arrival_airport = :airportCode "
                                                     "AND f.scheduled_arrival >= :date AND f.scheduled_arrival < (:date::timestamptz + interval '1 day') "
                                                     "ORDER BY scheduled_arrival";
    QString m_airPortCode;
    QDate m_date;
    FlightDirection m_direction;
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // FLIGHTDATAACCESSOR_H
