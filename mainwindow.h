#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QMainWindow>
#include <QMessageBox>
#include "airportdataaccessor.h"
#include "databaseconnection.h"
#include "flightdataaccessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_a_exit_triggered();

    void on_pb_showFlights_clicked();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent (QCloseEvent *event) override;

private:
    bool m_formShown = false;
    Ui::MainWindow *ui;
    DatabaseConnection* m_connection;
    AirportDataAccessor* m_airportAccessor;
    QSqlQueryModel* m_airportsModel;
    FlightDataAccessor* m_flightDataAccessor;
    QSqlQueryModel* m_flightsModel;
    QMessageBox* m_msg;
    bool m_airportsLoaded = false;
    void initInterface();
    void displayStatus(QString str);
    void connectToDatabase();
    void disconnectFromDatabase();
    void displayError(QString msg);
    void disableFilter();
    void enableFilter();
    void loadAirports();
    void displayAirports();
    void loadFlights();
    void adjustFlightView();

    inline static QString STATUS_CONNECTED = "Подключено";
    inline static QString STATUS_DISCONNECTED = "Отключено";
    inline static QDate MIN_DATE = QDate(2016, 8, 15);
    inline static QDate MAX_DATE = QDate(2017, 9, 14);
};

#endif // MAINWINDOW_H
