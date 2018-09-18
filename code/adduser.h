#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "database.h"
namespace Ui {
class adduser;
}

class adduser : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit adduser(QWidget *parent = 0);
    ~adduser();

private slots:
    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::adduser *ui;
};

#endif // ADDUSER_H
