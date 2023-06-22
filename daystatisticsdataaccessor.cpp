#include "daystatisticsdataaccessor.h"

DayStatisticsDataAccessor::DayStatisticsDataAccessor(DatabaseConnection *connection, QObject *parent)
    : DataAccessorBase{connection, parent}
{

}

const QString &DayStatisticsDataAccessor::getAirportCode() const
{
    return m_airPortCode;
}

void DayStatisticsDataAccessor::setAirportCode(const QString &airportCode)
{
    m_airPortCode = airportCode;
}

void DayStatisticsDataAccessor::setAirportCode(QString &&airportCode)
{
    m_airPortCode = airportCode;
}

void DayStatisticsDataAccessor::buildSql(QSqlQuery &query) const
{
    query.prepare(QUERY_TEXT);

    query.bindValue(":airportCode", m_airPortCode);
}

void DayStatisticsDataAccessor::initHeaders(QSqlQueryModel *model) const
{
    model->setHeaderData(0, Qt::Horizontal, tr("Количество рейсов"));
    model->setHeaderData(1, Qt::Horizontal, tr("День"));
}
