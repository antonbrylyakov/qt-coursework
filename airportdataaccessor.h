#ifndef AIRPORTDATAACCESSOR_H
#define AIRPORTDATAACCESSOR_H

#include "dataaccessorbase.h"

class AirportDataAccessor : public DataAccessorBase
{
public:
    AirportDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
protected:
    QString buildSql() override;
    void initHeaders(QSqlQueryModel* model) override;
};

#endif // AIRPORTDATAACCESSOR_H
