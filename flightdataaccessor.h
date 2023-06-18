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
    QString m_airPortCode;
    QDate m_date;
    FlightDirection m_direction;
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // FLIGHTDATAACCESSOR_H
