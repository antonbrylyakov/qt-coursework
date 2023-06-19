#ifndef STATISTICDIALOG_H
#define STATISTICDIALOG_H

#include <QDialog>

namespace Ui {
class statisticdialog;
}

class statisticdialog : public QDialog
{
    Q_OBJECT

public:
    explicit statisticdialog(QWidget *parent = nullptr);
    ~statisticdialog();

private:
    Ui::statisticdialog *ui;
};

#endif // STATISTICDIALOG_H
