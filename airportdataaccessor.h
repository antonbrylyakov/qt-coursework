#ifndef AIRPORTDATAACCESSOR_H
#define AIRPORTDATAACCESSOR_H

#include "dataaccessorbase.h"

class AirportDataAccessor : public DataAccessorBase
{
public:
    AirportDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
protected:
    inline static const QString QUERY_TEXT = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data ORDER BY airport_name->>'ru'";
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // AIRPORTDATAACCESSOR_H
