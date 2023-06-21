#include "statisticdialog.h"
#include "ui_statisticdialog.h"
#include <QBarSet>
#include <QSqlRecord>
#include <QDate>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QChart>
#include <QLineSeries>
#include <QMap>

StatisticDialog::StatisticDialog(DatabaseConnection *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticDialog)
{
    ui->setupUi(this);

    m_connection = connection;
    m_yearStatisticsDataAccessor = new YearStatisticsDataAccessor(m_connection, this);
    m_yearStatisticsModel = new QSqlQueryModel();
    m_dayStatisticsDataAccessor = new DayStatisticsDataAccessor(m_connection, this);
    m_dayStatisticsModel = new QSqlQueryModel();

    // При непредвиденных ошибках закроем окно
    connect(m_connection, &DatabaseConnection::sig_ConnectionError, [this]() { close(); });

    connect(m_yearStatisticsDataAccessor, &YearStatisticsDataAccessor::sig_DataReady,
            [this](auto model)
    {
        m_yearStatisticsLoaded = true;
        displayYearStatistics();
    });

    connect(m_dayStatisticsDataAccessor, &DayStatisticsDataAccessor::sig_DataReady,
            [this](auto model)
    {
        m_dayStatisticsLoaded = true;
        displayMonthStatistics();
    });

    m_yearChart = new QChart();
    ui->cv_yearChartView->setChart(m_yearChart);

    m_monthChart = new QChart();
    ui->cv_monthChartView->setChart(m_monthChart);
}

StatisticDialog::~StatisticDialog()
{
    delete ui;
    delete m_yearChart;
    delete m_monthChart;
}

void StatisticDialog::showStatistics(QString &&airportCode, QString &&airportName)
{
    m_yearChartShown = false;
    m_monthChartShown = false;
    ui->cb_month->setCurrentIndex(0);
    m_airportName = std::move(airportName);
    m_airportCode = std::move(airportCode);
    ui->lb_header->setText(QString("Загруженность по аэропорту %1").arg(m_airportName));
    setModal(true);
    ui->tw_statistics->setCurrentIndex(0);
    loadYearStatistics();
    show();
}

void StatisticDialog::on_tw_statistics_currentChanged(int index)
{
    if (index == 0)
    {
        if (m_yearStatisticsLoaded)
        {
            displayYearStatistics();
        }
        else
        {
            loadYearStatistics();
        }
    }
    else
    {
        if (m_dayStatisticsLoaded)
        {
            //ФТ21
            ui->cb_month->setCurrentIndex(0);
            displayMonthStatistics();
        }
        else
        {
            loadDayStatistics();
        }
    }
}

void StatisticDialog::displayYearStatistics()
{
    if (m_yearChartShown || !m_yearStatisticsLoaded)
    {
        return;
    }

    m_yearChart->removeAllSeries();
    auto oldAxes = m_yearChart->axes();
    for (auto axis: oldAxes)
    {
        m_yearChart->removeAxis(axis);
    }

    QBarSeries *series = new QBarSeries(m_yearChart);
    QBarSet *set = new QBarSet("Месяцы");
    QStringList months;
    int maxValue = 1;
    for (auto i = 0; i < m_yearStatisticsModel->rowCount(); ++i)
    {
        auto record = m_yearStatisticsModel->record(i);
        auto count = record.value(0).toInt();
        if (count > maxValue)
        {
            maxValue = count;
        }

        auto month = record.value(1).toDate();
        auto monthStr = locale().toString(month, "MMM");
        set->append(count);
        months.append(monthStr);
    }

    series->append(set);
    m_yearChart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    m_yearChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, 1.2 * maxValue);
    axisY->setLabelFormat("%d");
    axisY->applyNiceNumbers();
    m_yearChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_yearChart->legend()->setVisible(false);

    m_yearChartShown = true;
}

void StatisticDialog::displayMonthStatistics()
{
    if (m_monthChartShown || !m_dayStatisticsLoaded)
    {
        return;
    }

    fillMonths();

    auto monthNumber = ui->cb_month->currentData().toInt();

    m_monthChart->removeAllSeries();
    auto oldAxes = m_monthChart->axes();
    for (auto axis: oldAxes)
    {
        m_monthChart->removeAxis(axis);
    }

    QMap<int, int> dayMap;
    
    static const int defaultDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    auto daysInMonth = defaultDaysInMonth[monthNumber - 1];
    for (auto i = 0; i < m_dayStatisticsModel->rowCount(); ++i)
    {
        auto record = m_dayStatisticsModel->record(i);
        auto count = record.value(0).toInt();
        auto date = record.value(1).toDate();
        if (date.month() == monthNumber)
        {
            auto day = date.day();
            if (!dayMap.contains(day))
            {
                dayMap[day] = count;
            }
            else
            {
                dayMap[day] += count;
            }

            if (date.daysInMonth() > daysInMonth)
            {
                daysInMonth = date.daysInMonth();
            }
        }
    }

    auto maxValue = 1;
    QLineSeries* series = new QLineSeries(m_monthChart);
    for (auto day = 1; day <= daysInMonth; ++day)
    {
        auto it = dayMap.find(day);
        auto count = it != dayMap.end() ? it.value() : 0;
        series->append(day, count);

        if (count > maxValue)
        {
            maxValue = count;
       }
    }

    m_monthChart->addSeries(series);

    auto axisX = new QValueAxis();
    axisX->setRange(1, daysInMonth);
    axisX->setLabelFormat("%d");
    m_monthChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, 1.2 * maxValue);
    axisY->setLabelFormat("%d");
    axisY->applyNiceNumbers();
    m_monthChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_monthChart->legend()->setVisible(false);

    m_monthChartShown = true;
}

void StatisticDialog::loadYearStatistics()
{
    m_yearStatisticsDataAccessor->setAirportCode(m_airportCode);
    m_yearStatisticsDataAccessor->getData(m_yearStatisticsModel);
}

void StatisticDialog::loadDayStatistics()
{
    m_dayStatisticsDataAccessor->setAirportCode(m_airportCode);
    m_dayStatisticsDataAccessor->getData(m_dayStatisticsModel);
}

void StatisticDialog::fillMonths()
{
    if (m_monthsFilled)
    {
        return;
    }

    for (auto i = 1; i <= 12; ++i)
    {
        auto monthName = locale().standaloneMonthName(i);
        ui->cb_month->addItem(monthName, i);
    }

    m_monthsFilled = true;
}


void StatisticDialog::on_pb_close_clicked()
{
    close();
}


void StatisticDialog::on_cb_month_currentIndexChanged(int index)
{
    m_monthChartShown = false;
    displayMonthStatistics();
}

