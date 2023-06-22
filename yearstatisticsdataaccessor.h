#ifndef YEARSTATISTICSDATAACCESSOR_H
#define YEARSTATISTICSDATAACCESSOR_H

#include "dataaccessorbase.h"

class YearStatisticsDataAccessor : public DataAccessorBase
{
public:
    YearStatisticsDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
    const QString& getAirportCode() const;
    void setAirportCode(const QString& airportCode);
    void setAirportCode(QString&& airportCode);
protected:
    inline static const QString QUERY_TEXT = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                                             "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = :airportCode or arrival_airport = :airportCode ) "
                                             "group by \"Month\"";
    QString m_airPortCode;
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // YEARSTATISTICSDATAACCESSOR_H
