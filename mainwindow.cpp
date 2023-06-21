#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setLocale(QLocale(QLocale::Russian, QLocale::Russia));

    ui->setupUi(this);

    initInterface();

    m_msg = new QMessageBox(this);

    // ФТ2
    auto databaseConfig = DatabaseConfiguration
    {
        "981757-ca08998.tmweb.ru", 5432, "demo", "netology_usr_cpp", "CppNeto3"
    };

    m_connection = new DatabaseConnection(databaseConfig, this);
    m_airportAccessor = new AirportDataAccessor(m_connection, this);
    m_airportsModel = new QSqlQueryModel(this);
    m_flightDataAccessor = new FlightDataAccessor(m_connection, this);
    m_flightsModel = new QSqlQueryModel(this);
    ui->tv_flights->setModel(m_flightsModel);

    displayStatus(STATUS_DISCONNECTED);
    connect(m_connection, &DatabaseConnection::sig_ChangeConnectionStatus, [this](auto status)
    {
        if (status == DatabaseConnectionStatus::Connected)
        {
            displayStatus(STATUS_CONNECTED);
            if (!m_airportsLoaded)
            {
                loadAirports();
            }
        }
        else
        {
            displayStatus(STATUS_DISCONNECTED);
        }
    });

    connect(m_connection, &DatabaseConnection::sig_ConnectionError, this, &MainWindow::displayError);

    connect(m_airportAccessor, &AirportDataAccessor::sig_DataReady, this, &MainWindow::displayAirports);

    connect(m_airportAccessor, &AirportDataAccessor::sig_QueryError, this, &MainWindow::displayError);

    connect(m_flightDataAccessor, &FlightDataAccessor::sig_QueryError, this, &MainWindow::displayError);

    connect(m_flightDataAccessor, &FlightDataAccessor::sig_DataReady, this, &MainWindow::adjustFlightView);

    m_statDialog = new StatisticDialog(m_connection, this);
    m_statDialog->setLocale(locale());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_a_exit_triggered()
{
    close();
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    connectToDatabase();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    disconnectFromDatabase();
}

void MainWindow::initInterface()
{
    ui->lb_help->setVisible(true);
    ui->tv_flights->setVisible(false);
    // ФТ4
    disableFilter();
    ui->rb_departure->setChecked(true);
    // О2
    ui->de_flightDate->setMinimumDate(MIN_DATE);
    ui->de_flightDate->setMaximumDate(MAX_DATE);
}

void MainWindow::displayStatus(QString str)
{
    // ФТ3
    ui->statusbar->showMessage(str);
}

void MainWindow::connectToDatabase()
{
    m_connection->open();
}

void MainWindow::disconnectFromDatabase()
{
    m_connection->close();
}

void MainWindow::displayError(QString msg)
{
    disableFilter();
    m_msg->setIcon(QMessageBox::Critical);
    m_msg->setText(msg);
    m_msg->exec();

    // ФТ6
    m_connection->open(true);
}

void MainWindow::disableFilter()
{
    ui->gb_filter->setEnabled(false);
}

void MainWindow::enableFilter()
{
    ui->gb_filter->setEnabled(true);
}

void MainWindow::loadAirports()
{
    m_airportAccessor->getData(m_airportsModel);
}

void MainWindow::displayAirports()
{
    m_airportsLoaded = true;
    auto cb = ui->cbx_airport;
    cb->clear();
    for (auto i = 0; i < m_airportsModel->rowCount(); ++i)
    {
        auto record = m_airportsModel->record(i);
        auto name = record.value(0).toString();
        auto code = record.value(1);
        auto text = QString("%1 (%2)").arg(name, code.toString());
        cb->addItem(text, code);
    }

    m_airportsModel->clear();
    enableFilter();
}

void MainWindow::loadFlights()
{
    disableFilter();
    m_flightDataAccessor->setAirportCode(ui->cbx_airport->currentData().toString());
    m_flightDataAccessor->setDate(ui->de_flightDate->date());
    m_flightDataAccessor->setFlightDirection(ui->rb_arrival->isChecked() ? FlightDirection::Arrival : FlightDirection::Departure);
    m_flightDataAccessor->getData(m_flightsModel);
}

void MainWindow::adjustFlightView()
{
    enableFilter();
    if (m_flightsModel->rowCount() > 0)
    {
        ui->lb_help->setVisible(false);
        ui->tv_flights->setVisible(true);
        ui->tv_flights->resizeColumnsToContents();
    }
    else
    {
        ui->lb_help->setText(NO_RESULTS);
        ui->lb_help->setVisible(true);
        ui->tv_flights->setVisible(false);
    }
}


void MainWindow::on_pb_showFlights_clicked()
{
    loadFlights();
}


void MainWindow::on_pb_load_clicked()
{
    m_statDialog->showStatistics(ui->cbx_airport->currentData().toString(),  ui->cbx_airport->currentText());
}

