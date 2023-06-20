#include "statisticdialog.h"
#include "ui_statisticdialog.h"
#include <QBarSet>
#include <QSqlRecord>
#include <QDate>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QChart>

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
    m_yearChartView = new QChartView(m_yearChart, this);
    ui->tb_year->layout()->addWidget(m_yearChartView);
}

StatisticDialog::~StatisticDialog()
{
    delete ui;
    delete m_yearChart;
}

void StatisticDialog::showStatistics(QString &&airportCode, QString &&airportName)
{
    m_yearChartShown = false;
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
    if (m_yearChartShown)
    {
        return;
    }

    m_yearChart->removeAllSeries();
    auto oldAxes = m_yearChart->axes();
    for (auto axis: oldAxes)
    {
        m_yearChart->removeAxis(axis);
    }

    QBarSeries *series = new QBarSeries();
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
        auto monthStr = QLocale::system().toString(month, "MMM");
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


void StatisticDialog::on_pb_close_clicked()
{
    close();
}

