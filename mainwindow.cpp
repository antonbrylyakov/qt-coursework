#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ФТ4
    disableFilter();

    m_msg = new QMessageBox(this);

    // ФТ2
    auto databaseConfig = DatabaseConfiguration
    {
        "981757-ca08998.tmweb.ru", 5432, "demo", "netology_usr_cpp", "CppNeto3"
    };

    m_connection = new DatabaseConnection(databaseConfig, this);

    displayStatus(STATUS_DISCONNECTED);
    connect(m_connection, &DatabaseConnection::sig_ChangeConnectionStatus, [this](auto status)
    {
        if (status == DatabaseConnectionStatus::Connected)
        {
            displayStatus(STATUS_CONNECTED);
        }
        else
        {
            displayStatus(STATUS_DISCONNECTED);
        }
    });

    connect(m_connection, &DatabaseConnection::sig_ConnectionError, this, &MainWindow::displayError);
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

