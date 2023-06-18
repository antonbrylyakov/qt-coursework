#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "airportdataaccessor.h"
#include "databaseconnection.h"

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

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent (QCloseEvent *event) override;

private:
    bool m_formShown = false;
    Ui::MainWindow *ui;
    DatabaseConnection* m_connection;
    AirportDataAccessor* m_airportAccessor;
    QSqlQueryModel* m_airportsModel;
    QMessageBox* m_msg;
    bool m_airportsLoaded = false;
    void displayStatus(QString str);
    void connectToDatabase();
    void disconnectFromDatabase();
    void displayError(QString msg);
    void disableFilter();
    void enableFilter();
    void loadAirports();
    void displayAirports();

    inline static QString STATUS_CONNECTED = "Подключено";
    inline static QString STATUS_DISCONNECTED = "Отключено";
};

#endif // MAINWINDOW_H
