#ifndef AIRPORTDATAACCESSOR_H
#define AIRPORTDATAACCESSOR_H

#include "dataaccessorbase.h"

class AirportDataAccessor : public DataAccessorBase
{
public:
    AirportDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
protected:
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // AIRPORTDATAACCESSOR_H
