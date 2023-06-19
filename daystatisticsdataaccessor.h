#ifndef DAYSTATISTICSDATAACCESSOR_H
#define DAYSTATISTICSDATAACCESSOR_H

#include "dataaccessorbase.h"

class DayStatisticsDataAccessor : public DataAccessorBase
{
public:
    DayStatisticsDataAccessor(DatabaseConnection* connection, QObject *parent = nullptr);
    const QString& getAirportCode() const;
    void setAirportCode(const QString& airportCode);
    void setAirportCode(QString&& airportCode);
protected:
    QString m_airPortCode;
    void buildSql(QSqlQuery& query) const override;
    void initHeaders(QSqlQueryModel* model) const override;
};

#endif // DAYSTATISTICSDATAACCESSOR_H
