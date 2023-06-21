#ifndef STATISTICDIALOG_H
#define STATISTICDIALOG_H

#include <QDialog>
#include <QChartView>
#include <QSqlQueryModel>
#include <QBarSeries>
#include <QLocale>
#include "databaseconnection.h"
#include "yearstatisticsdataaccessor.h"
#include "daystatisticsdataaccessor.h"

namespace Ui {
class StatisticDialog;
}

class StatisticDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticDialog(DatabaseConnection* connection, QWidget *parent = nullptr);
    ~StatisticDialog();
    void showStatistics(QString&& airportCode, QString&& airportName);

private slots:
    void on_tw_statistics_currentChanged(int index);
    void displayYearStatistics();
    void displayMonthStatistics();

    void on_pb_close_clicked();

    void on_cb_month_currentIndexChanged(int index);

private:
    Ui::StatisticDialog *ui;
    DatabaseConnection* m_connection;
    YearStatisticsDataAccessor* m_yearStatisticsDataAccessor;
    QSqlQueryModel* m_yearStatisticsModel;
    DayStatisticsDataAccessor* m_dayStatisticsDataAccessor;
    QSqlQueryModel* m_dayStatisticsModel;
    QString m_airportName;
    QString m_airportCode;
    bool m_yearStatisticsLoaded = false;
    bool m_dayStatisticsLoaded = false;
    QChart* m_yearChart;
    bool m_yearChartShown = false;
    QChart* m_monthChart;
    bool m_monthChartShown;
    bool m_monthsFilled = false;

    void loadYearStatistics();
    void loadDayStatistics();
    void fillMonths();
    //QBarSeries* createYearSeries();
};

#endif // STATISTICDIALOG_H
