#include "yearstatisticsdataaccessor.h"

YearStatisticsDataAccessor::YearStatisticsDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

const QString &YearStatisticsDataAccessor::getAirportCode() const
{
    return m_airPortCode;
}

void YearStatisticsDataAccessor::setAirportCode(const QString &airportCode)
{
    m_airPortCode = airportCode;
}

void YearStatisticsDataAccessor::setAirportCode(QString &&airportCode)
{
    m_airPortCode = airportCode;
}

void YearStatisticsDataAccessor::buildSql(QSqlQuery &query) const
{
    query.prepare(QUERY_TEXT);

    query.bindValue(":airportCode", m_airPortCode);
}

void YearStatisticsDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Количество рейсов"));
    model->setHeaderData(1, Qt::Horizontal, tr("Месяц"));
}


