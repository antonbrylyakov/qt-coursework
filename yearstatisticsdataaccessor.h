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
    QString m_airPortCode;
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // YEARSTATISTICSDATAACCESSOR_H
