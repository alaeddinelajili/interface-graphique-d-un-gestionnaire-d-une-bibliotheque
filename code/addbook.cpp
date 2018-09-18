#include "addbook.h"
#include "ui_addbook.h"
#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QSqlError>
addbook::addbook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addbook)
{
    ui->setupUi(this);
}

addbook::~addbook()
{
    delete ui;
}

void addbook::on_save_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("INSERT INTO Books ('Titre','SousTitre','Domaine','Nombre_copie','Date','Auteur','Cover')"
                "VALUES (:titre, :soustitre, :domaine, :nb_copie, :date, :aut,:file)");

    qry.bindValue(":titre", ui->titre->text());
    qry.bindValue(":soustitre", ui->soustitre->text());
    qry.bindValue(":domaine", ui->domaine->text());
    qry.bindValue(":nb_copie", ui->nb->text());
    qry.bindValue(":date", ui->date->text());
    qry.bindValue(":aut", ui->auteur->text());
     //si date ne marche pas voir ça
    //qry.bindValue(":date", ui->date_ins->date().toString());



    QFile  file(imgPath);

    if(!file.open(QIODevice::ReadOnly))
        return;


    qry.bindValue(":file",file.readAll());


    if(!qry.exec()){
        qDebug() << qry.lastError() ;
    }else{
        qDebug() << "succes saved new book";
    }


    dbclose();
    close();
}

void addbook::on_cancel_clicked()
{
    close();
}

void addbook::on_cover_clicked()
{
    imgPath = QFileDialog::getOpenFileName(this,tr("open file"),"/home", tr("Images (*.png *.jpeg *.jpg)"));
}
