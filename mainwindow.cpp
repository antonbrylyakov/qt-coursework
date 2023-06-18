#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
        auto text = QString("%1 (%2)").arg(name).arg(code.toString());
        cb->addItem(text, code);
    }

    m_airportsModel->clear();
    enableFilter();
}

