#include "statisticdialog.h"
#include "ui_statisticdialog.h"

statisticdialog::statisticdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statisticdialog)
{
    ui->setupUi(this);
}

statisticdialog::~statisticdialog()
{
    delete ui;
}
