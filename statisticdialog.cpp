#include "statisticdialog.h"
#include "ui_statisticdialog.h"

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
}

StatisticDialog::~StatisticDialog()
{
    delete ui;
}

void StatisticDialog::showStatistics(QString &&airportCode, QString &&airportName)
{
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

