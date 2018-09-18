#ifndef ADDBOOK_H
#define ADDBOOK_H
#include<database.h>
#include <QDialog>

namespace Ui {
class addbook;
}

class addbook : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit addbook(QWidget *parent = 0);
    ~addbook();

private slots:
    void on_save_clicked();

    void on_cancel_clicked();

    void on_cover_clicked();

private:
    Ui::addbook *ui;
    QString imgPath;
};

#endif // ADDBOOK_H
