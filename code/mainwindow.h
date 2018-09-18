#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QListWidgetItem>
#include <QImage>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, protected Database
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();

    void on_log_out2_clicked();

    void on_BookView_clicked(const QModelIndex &index);

    void on_recherche_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_chek_avancer_stateChanged(int arg1);

    void on_emp_clicked();

    void on_TakeBook_clicked(const QModelIndex &index);

    void on_addUser_clicked();

    void on_DelUser_clicked();

    void on_AdminUsers_clicked(const QModelIndex &index);

    void on_alert_clicked();

    void on_back_clicked();

    void on_Book_clicked(const QModelIndex &index);

    void on_chek_avancer_2_stateChanged(int arg1);

    void on_recherche_2_clicked();

    void on_AdminBook_clicked(const QModelIndex &index);

    void on_deleteBook_clicked();

    void on_rendreBook_clicked();

    void on_mod_book_clicked();

    void on_addBook_clicked();

private:
    Ui::MainWindow *ui;
    QString user;
    QString titre,soustitre,domaine,date,auteur;
    QPixmap cover;
    QString id11;
    QString userName;
    QString id;
    int id_book;
    int id_taked;
};

#endif // MAINWINDOW_H
