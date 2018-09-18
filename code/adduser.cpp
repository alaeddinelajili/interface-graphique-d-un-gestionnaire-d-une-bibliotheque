#include "adduser.h"
#include "ui_adduser.h"
#include <QDebug>
#include <QSqlError>
#include <QBuffer>
#include <QFileDialog>
#include <QString>
#include <QPixmap>
adduser::adduser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adduser)
{
    ui->setupUi(this);
    ui->date_ins->setDate(QDate::currentDate());
}

adduser::~adduser()
{
    delete ui;
}

void adduser::on_cancel_clicked()
{
     close();
}

void adduser::on_save_clicked() //insert into the base the user information
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("INSERT INTO UserInfo ('Pseudo','Password','Rang','Nom','Prenom','Adresse','Tel','Mail','Date_Ins')"
                "VALUES (:pseudo, :password, :rang, :nom, :prenom, :adresse, :tel, :mail, :date)");
    qry.bindValue(":pseudo", ui->pseu->text());
    qry.bindValue(":password", ui->pass->text());
    qry.bindValue(":rang", ui->stat->currentText());
    qry.bindValue(":nom", ui->user_nom->text());
    qry.bindValue(":prenom", ui->user_pre->text());
    qry.bindValue(":adresse", ui->addr->text());
    qry.bindValue(":tel", ui->tel->text());
    qry.bindValue(":mail", ui->email->text());
    qry.bindValue(":date", ui->date_ins->date().toString("dd/MM/yyyy"));

    if(!qry.exec()){
        qDebug() << qry.lastError() ;
    }else{
        qDebug() << "succes saved new user";
    }
    dbclose();
    close();
}
