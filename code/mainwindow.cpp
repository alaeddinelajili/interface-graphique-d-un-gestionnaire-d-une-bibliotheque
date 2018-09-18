#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QDebug>
#include <QDir>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QInputDialog>
#include<QString>
#include<adduser.h>
#include<addbook.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() // fonction login, il y a deux interfaces, user et admin qui est capable de gerer le contenu du bibliotheque

{
    QString pseudo,password;

    pseudo = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if (!dbConnect()){ //verifier si l'utilisateur est connecté à la BD
            qDebug() << " not  Connected to database";
            return;
        }
        QSqlQuery qry;
        qry.prepare("SELECT Pseudo,Password,Rang,user_id,Nom,Prenom FROM UserInfo WHERE Pseudo='"+pseudo+
                    "' AND Password='"+password+"';");

        if (qry.exec()){

            if(qry.next()){

                qDebug() << "username : " << qry.value(0).toString() << "\npassword : "
                         << qry.value(1).toString() <<
                          "admin "<< qry.value(2).toString();
                if (qry.value(2).toString()=="Admin"){ // si l'utilisateur est  l'admin
                    ui->stackedWidget->setCurrentIndex(1);

                    QSqlQuery qry;
                    QSqlQueryModel *model=new QSqlQueryModel();
                    qry.prepare("SELECT * FROM Books");
                    qry.exec();
                    model->setQuery(qry);
                    ui->AdminBook->setModel(model);
                    ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

                }else{ //l'utilisateur est un etudiant ou un enseignant
                    ui->stackedWidget->setCurrentIndex(2);
                    ui->stackedWidget_3->setCurrentIndex(0);

                    user = qry.value(4).toString()+" "+qry.value(5).toString();
                    userName=qry.value(4).toString();
                    id = qry.value(3).toString(); // collect the user id
                    ui->user->setText("Bienvenue cher "+user); //afficher le nome de user
                    ui->status->setText(qry.value(2).toString());
                    //view data in the listview
                    QSqlQueryModel *modal=new QSqlQueryModel();

                    qry.prepare("SELECT Titre,SousTitre,Domaine,Auteur FROM Books"); //afficher tous les livres
                    qry.exec();
                    modal->setQuery(qry);
                    ui->BookView->setModel(modal);
                    ui->BookView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

                    // show a warning alert if there is a book that the user forget to return
                    qry.prepare("SELECT * FROM taked WHERE (etudiant_id LIKE :Userid) AND (date_rendre BETWEEN datetime('now','start of day','-2 month','-1 day') AND datetime('now')) ");
                    qry.bindValue(":Userid", id);
                    if(qry.exec()){
                    int nb=0;
                    while (qry.next()){nb++;}
                    if (nb==0){ui->alert->setDisabled(1);}
                    else{ui->alert->setEnabled(1); // make the alert bouton enabel if there is a one
                    QMessageBox::warning(this,"Alert","check your alert box there is a book you didn't returned");}
                }}
                dbclose();

            }else{

                qDebug() << "username or password are incorrect";
                dbclose();

            }
        }
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");

       }

void MainWindow::on_log_out2_clicked() //quitter la biblio
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_BookView_clicked(const QModelIndex &index)
{
    //dans cette fonciton on vas afficher les info d'un livre  dans la partie utilisateur
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT Titre,Cover,SousTitre FROM Books WHERE Titre='"+index.sibling(index.row(), 0).data().toString()+"' AND SousTitre='"+index.sibling(index.row(), 1).data().toString()+"'");
    qry.exec();
    qry.next();
    QByteArray array = qry.value(1).toByteArray();
    QPixmap cov;
    cover.loadFromData(array);
    cov.loadFromData(array);

    ui->Couverture->setPixmap(cov);
    ui->Couverture->setScaledContents(true);

    titre=qry.value(0).toString();
    soustitre=qry.value(2).toString();

    ui->titre->setText(titre);
    ui->desc->setText(soustitre);
    dbclose();
}

void MainWindow::on_recherche_clicked()
{
        // Bouton pour la recherche d'un livre
        dbConnect();
        QSqlQuery qry;
        QSqlQueryModel *modal=new QSqlQueryModel();
        if(ui->chek_avancer->isChecked() == true) //pour un recherche avancer
        {
            qDebug() << ui->Domaine_2->currentText();
            if(ui->Date->text()=="" && ui->Auteur->text()=="")
            {
                 qry.prepare("SELECT Titre,SousTitre,Domaine,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom");
                 qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
                 qry.bindValue(":dom", ui->Domaine_2->currentText());
            }else if((ui->Date->text()=="") && (ui->Auteur->text()!="")) {
                qry.prepare("SELECT Titre,SousTitre,Domaine,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Auteur LIKE :au");
                qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
                qry.bindValue(":dom", ui->Domaine_2->currentText());
                qry.bindValue(":au", "%"+ui->Auteur->text()+"%");
            }else if((ui->Date->text()!="") && (ui->Auteur->text()=="")){

                qry.prepare("SELECT Titre,SousTitre,Domaine,Date,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Date LIKE :da");
                qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
                qry.bindValue(":dom", ui->Domaine_2->currentText());
                qry.bindValue(":au", "%"+ui->Date->text()+"%");

            }else{
                qry.prepare("SELECT Titre,SousTitre,Domaine,Date,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Auteur LIKE :au AND Date LIKE :da");
                qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
                qry.bindValue(":dom", ui->Domaine_2->currentText());
                qry.bindValue(":au", "%"+ui->Auteur->text()+"%");
                qry.bindValue(":da", "%"+ui->Date->text()+"%");
            }
        }

        else //pour un recherche simple selon le titre de livre
        {
            qry.prepare("SELECT Titre,SousTitre,Domaine,Auteur FROM Books WHERE Titre LIKE :ti");
            qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
        }
        qry.exec();

        modal->setQuery(qry);
        ui->BookView->setModel(modal);
        ui->BookView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
        dbclose();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item) // pour la gestion de l'interface admin
{
    if (!(ui->listWidget->row(item)==3)){
        //Aller vers la prtie selectioné pour la gestion
        int partie =  ui->listWidget->row(item);
        ui->stackedWidget_2->setCurrentIndex(partie);

        dbConnect();

        QSqlQuery qry;
        QSqlQueryModel *model=new QSqlQueryModel();

        switch (partie){
            case 0:{
                qry.prepare("SELECT * FROM Books");
                qry.exec();
                model->setQuery(qry);
                ui->AdminBook->setModel(model);
                ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
                break;
            }
            case 1:{
                //liste des livres emprunté:
                qry.prepare("SELECT * FROM Taked");
                qry.exec();
                model->setQuery(qry);
                ui->TakeBook->setModel(model);
                ui->TakeBook->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                break;
            }
            case 2:{
<<<<<<< HEAD
                //liste des users"
=======
                //liste des users";
>>>>>>> 02f0af2404ecd88c2fa5245ae04ada9c5700d253
                qry.prepare("SELECT * FROM UserInfo");
                id=
                qry.exec();
                model->setQuery(qry);
                ui->AdminUsers->setModel(model);
                ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
                break;
            }
        }
    }else{
        // Revenir vers la page d'indentification
        ui->stackedWidget->setCurrentIndex(0);
    }
    dbclose();
}

void MainWindow::on_chek_avancer_stateChanged(int) //pour choisir entre recherche avancer et recherche simple
{
    if(ui->chek_avancer->isChecked() == true)
    {
        ui->Domaine_2->setEnabled(1);
        ui->Date->setEnabled(1);
        ui->Auteur->setEnabled(1);
    }

    else
    {
        ui->Domaine_2->setDisabled(1);
        ui->Date->setDisabled(1);
        ui->Auteur->setDisabled(1);
    }
}

void MainWindow::on_emp_clicked() //avant de clicker emprunter bookView doit etre clicked
{
    QMessageBox::critical(this, "emprunter", "monsieur " +user+ " vous etes entrain d'emprunter le livre " +titre+ " pour une durée maximale de 1 mois ");

    dbConnect();
    QSqlQuery qry1; //pour la verification de nombre d'exemplaires
    int nb_Exmplaires;
    qry1.prepare("SELECT Nombre_copie FROM Books WHERE Titre='"+titre+"' ");
    if( !qry1.exec() )
       qDebug() << qry1.lastError();
    else
       qDebug( "SElected" );

    qry1.next();
    nb_Exmplaires=qry1.value(0).toInt();
    if (nb_Exmplaires>0)
       {

            QSqlQuery qry; //pour la selection de book_id
            QSqlQuery qryy; // pour la selection de etudiant_id
            QSqlQuery qryyy; // pour l'insertion de tous les donnees necessaire sur l'emprunt
            QSqlQuery qry2;  // pour desincrementer le nombre d'exemplaires
            qry.prepare("SELECT ID FROM Books WHERE Titre='"+titre+"' ");
            if( !qry.exec() )
                qDebug() << qry.lastError();
            else
                qDebug( "SElected1" );
            qry.next();
            id_book=qry.value(0).toInt();
            qDebug() << "id book: " << id_book ;

            qryy.prepare("SELECT user_Id FROM UserInfo WHERE Nom='"+userName+"' ");
            if( !qryy.exec() )
                qDebug() << qryy.lastError();
            else
                qDebug( "SElected2" );
            qryy.next();
            id=qryy.value(0).toString();
            qDebug() << "id : " << id ;

            qryyy.prepare("INSERT INTO taked (book_id,etudiant_id,date_emp,date_rendre) VALUES (:book,:user,datetime('now'), datetime('now','start of day','+1 month','-1 day')) ");

            // l'utilisateur doit rendre le livre apres avoir emprunter le livre d'une durée qui ne depasse pas un mois

            qryyy.bindValue(":book",id_book);
            qryyy.bindValue(":user",id);
            if( !qryyy.exec() )
                qDebug() << qryyy.lastError();
            else
                qDebug( "Insered" );

            qry2.prepare("UPDATE Books SET Nombre_copie=:nb WHERE Titre='"+titre+"' ");
            qry2.bindValue(":nb",nb_Exmplaires-1);
            if( !qry2.exec() )
                qDebug() << qry2.lastError();
            else
                qDebug( "Updated" );

            QMessageBox::information(this,"emprunter","operation faite avec succes !");
    }

   else
            QMessageBox::critical(this,"emprunter","le Livre " +titre+ " est indisponible dans notre stock pour le moment veuillez consulter notre bibliothque plus tard");



}


void MainWindow::on_TakeBook_clicked(const QModelIndex &index)
{
    dbConnect();
    QSqlQuery qry; // see the user information, who tooked the book
    qry.prepare("SELECT Nom,Prenom,Rang FROM UserInfo WHERE user_id="+index.sibling(index.row(), 2).data().toString()); //afficher le nom et le prenom de l'utilisateur qui empreinte le livre
    qry.exec();
    qry.next();
    ui->NomEtudiant->setText(qry.value(0).toString());
    ui->PrenomEtudiant->setText(qry.value(1).toString());
    ui->RangEtudiant->setText(qry.value(2).toString());
    // see the tooked book
    qry.prepare("SELECT Titre,SousTitre,Domaine,Cover FROM Books WHERE ID="+index.sibling(index.row(), 1).data().toString()); //afficher le livre emp
    qry.exec();
    qry.next();
    ui->TitreLivre->setText(qry.value(0).toString());
    ui->SubLivre->setText(qry.value(1).toString());
    ui->DomLivre->setText(qry.value(2).toString());

    QByteArray array = qry.value(3).toByteArray(); //affiche l'image de cov
    QPixmap cover= QPixmap();
    cover.loadFromData(array);
    ui->CovLivre->setPixmap(cover);
    ui->CovLivre->setScaledContents(true);
    QSqlQuery qryy; // to select id_taked
    qryy.prepare("SELECT ID FROM taked WHERE ID='"+index.sibling(index.row(),0).data().toString()+"' ");
    qryy.exec();
    qryy.next();
    id_taked =qryy.value(0).toInt();

    dbclose();
}

void MainWindow::on_addUser_clicked()
{
    // Affichage de la fenetre pour ajouter un utilisateur
    adduser add;
    add.setModal(true);
    if (add.exec()){

    }
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *model= new QSqlQueryModel();
    qry.prepare("SELECT * FROM UserInfo"); //refraichir le liste de utilisateurs
    qry.exec();
    model->setQuery(qry);
    ui->AdminUsers->setModel(model);
    ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_AdminUsers_clicked(const QModelIndex &index)   // pour la selection de id de user
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT user_Id FROM UserInfo WHERE user_id="+index.sibling(index.row(), 0).data().toString());
    qry.exec();
    qry.next();
    id=qry.value(0).toString();
}
void MainWindow::on_DelUser_clicked() //avant de clicker supprimer, user must be clicked
{
    //supprimer un Utilisateur
    dbConnect();
    QMessageBox::StandardButton msgBox;
    msgBox= QMessageBox::question(this, "supprimer utilisateur", "Voullez vous vraiment supprimer cet utilisateur ??", QMessageBox::Yes|QMessageBox::No);
    if(msgBox == QMessageBox::Yes){
        QSqlQuery qry;
        qry.prepare("DELETE FROM UserInfo WHERE user_Id LIKE :id");
        qry.bindValue(":id", id);
        qry.exec();

        QSqlQueryModel *modal=new QSqlQueryModel(); //reflechir le liste de utilisateurs
        qry.prepare("SELECT * FROM UserInfo");
        qry.exec();
        modal->setQuery(qry);
        ui->AdminUsers->setModel(modal);
        ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

    }
    dbclose();
}



void MainWindow::on_alert_clicked()
{
    // Affichage de la listes des livres en retard
    ui->stackedWidget_3->setCurrentIndex(1);
    dbConnect();
    QSqlQuery qryy2;
    qryy2.prepare("SELECT user_Id FROM UserInfo WHERE Nom='"+userName+"' ");
    qryy2.exec();
    qryy2.next();
    id11=qryy2.value(0).toString();
    QSqlQuery qry;


    QSqlQueryModel *model= new QSqlQueryModel();
    qry.prepare("SELECT * FROM taked WHERE (etudiant_id LIKE :Userid) AND (date_rendre BETWEEN datetime('now','start of day','-2 month','-1 day') AND datetime('now')) ");
    qry.bindValue(":Userid", id11);
    qry.exec();
    model->setQuery(qry);
    ui->Book->setModel(model);
    ui->Book->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_back_clicked() // back to welcome page
{
   ui->stackedWidget_3->setCurrentIndex(0);
}


void MainWindow::on_Book_clicked(const QModelIndex &index)
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT Titre,SousTitre,Domaine,Cover FROM Books WHERE ID="+index.sibling(index.row(), 1).data().toString()); //afficher le livre emp
    qry.exec();
    qry.next();
    ui->Titre->setText(qry.value(0).toString());
    ui->Sub->setText(qry.value(1).toString());
    ui->Dom->setText(qry.value(2).toString());

    QByteArray array = qry.value(3).toByteArray(); //affiche l'image de cov
    QPixmap cover= QPixmap();
    cover.loadFromData(array);
    ui->cov->setPixmap(cover);
    ui->cov->setScaledContents(true);

    dbclose();
}

void MainWindow::on_chek_avancer_2_stateChanged(int) //pour choisir entre recherche avanceé et recherche simple
{
    if(ui->chek_avancer_2->isChecked() == true)
    {
        ui->Domaine->setEnabled(1);
        ui->Date_2->setEnabled(1);
        ui->auteur_2->setEnabled(1);
    }

    else
    {
        ui->Domaine->setDisabled(1);
        ui->Date_2->setDisabled(1);
        ui->auteur_2->setDisabled(1);
    }
}

void MainWindow::on_recherche_2_clicked()
{
    // Bouton pour la recherche d'un livre
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *modal=new QSqlQueryModel();
    if(ui->chek_avancer_2->isChecked() == true) //pour une recherche avancée
    {
        qDebug() << ui->Domaine->currentText();
        if(ui->Date_2->text()=="" && ui->auteur_2->text()=="")
        {
             qry.prepare("SELECT Titre,SousTitre,Domaine,Nombre_copie,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom");
             qry.bindValue(":ti", "%"+ui->titre_livre_2->text()+"%");
             qry.bindValue(":dom", ui->Domaine->currentText());
        }else if((ui->Date_2->text()=="") && (ui->auteur_2->text()!="")) {
            qry.prepare("SELECT Titre,SousTitre,Domaine,Nombre_copie,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Auteur LIKE :au");
            qry.bindValue(":ti", "%"+ui->titre_livre_2->text()+"%");
            qry.bindValue(":dom", ui->Domaine->currentText());
            qry.bindValue(":au", "%"+ui->auteur_2->text()+"%");
        }else if((ui->Date_2->text()!="") && (ui->auteur_2->text()=="")){

            qry.prepare("SELECT Titre,SousTitre,Domaine,Nombre_copie,Date,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Date LIKE :da");
            qry.bindValue(":ti", "%"+ui->titre_livre_2->text()+"%");
            qry.bindValue(":dom", ui->Domaine->currentText());
            qry.bindValue(":au", "%"+ui->Date_2->text()+"%");

        }else{
            qry.prepare("SELECT Titre,SousTitre,Domaine,Nombre_copie,Date,Auteur FROM Books WHERE Titre LIKE :ti AND Domaine LIKE :dom AND Auteur LIKE :au AND Date LIKE :da");
            qry.bindValue(":ti", "%"+ui->titre_livre_2->text()+"%");
            qry.bindValue(":dom", ui->Domaine->currentText());
            qry.bindValue(":au", "%"+ui->auteur_2->text()+"%");
            qry.bindValue(":da", "%"+ui->Date_2->text()+"%");
        }
    }

    else //pour une recherche simple selon le titre de livre
    {
        qry.prepare("SELECT Titre,SousTitre,Domaine,Nombre_copie,Auteur FROM Books WHERE Titre LIKE :ti");
        qry.bindValue(":ti", "%"+ui->titre_livre_2->text()+"%");
    }
    qry.exec();

    modal->setQuery(qry);
    ui->AdminBook->setModel(modal);
    ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_AdminBook_clicked(const QModelIndex &index)
{
    dbConnect();
    QSqlQuery qry; // select book's informations
    qry.prepare("SELECT ID,Titre FROM Books WHERE Titre='"+index.sibling(index.row(), 1).data().toString()+"' ");
    qry.exec();
    qry.next();
    id_book=qry.value(0).toInt();
    titre=qry.value(1).toString();

    qDebug() << id_book;
    qDebug() << titre;
    dbclose();

}

void MainWindow::on_addBook_clicked()
{
    addbook add;
    add.setModal(true);
    if (add.exec()){

    }
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *model= new QSqlQueryModel();
    qry.prepare("SELECT * FROM Books");
    qry.exec();
    model->setQuery(qry);
    ui->AdminBook->setModel(model);
    ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_deleteBook_clicked()
{
    //supprimer un livre
    dbConnect();
    QMessageBox::StandardButton msgBox;
    msgBox= QMessageBox::question(this, "delete book", "Voullez vous vraiment supprimer ce livre ??",
                                  QMessageBox::Yes|QMessageBox::No);
    if(msgBox == QMessageBox::Yes){

        QSqlQuery qry;
        qry.prepare("DELETE FROM Books WHERE ID LIKE :id");
        qry.bindValue(":id", id_book);
        qDebug() << id_book ;

        if( !qry.exec() )
            qDebug() << qry.lastError();                                            //ca marche pas
        else
            qDebug( "DELETED" );

        QSqlQueryModel *modal=new QSqlQueryModel();
        qry.prepare("SELECT * FROM Books");
        qry.exec();
        modal->setQuery(qry);
        ui->AdminBook->setModel(modal);
        ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

    }
    dbclose();
}

void MainWindow::on_rendreBook_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("DELETE FROM taked WHERE ID LIKE :id");
    qry.bindValue(":id", id_taked);
    qry.exec();
    qDebug("DELETED") ;

    QSqlQuery qryy;
    QSqlQueryModel *model=new QSqlQueryModel();
    qryy.prepare("SELECT * FROM Taked");
    qryy.exec();
    model->setQuery(qryy);
    ui->TakeBook->setModel(model);
    ui->TakeBook->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int nbr;
    QSqlQuery qry1;
    qry1.prepare("SELECT Nombre_copie FROM Books WHERE ID LIKE :id");
    qry1.bindValue(":id",id_book);
    if( !qry1.exec() )
       qDebug() << qry1.lastError();
    else
       qDebug( "SElected" );

    qry1.next();
    nbr=qry1.value(0).toInt();
    qDebug() <<  nbr;
    QSqlQuery qry2;//mettre à jour le nombre de copies
    qry2.prepare("UPDATE Books SET Nombre_copie=:nb WHERE ID LIKE :id");
    qry2.bindValue(":id", id_book);
    qry2.bindValue(":nb",nbr+1);
    if( !qry2.exec() )
        qDebug() << qry2.lastError();
    else
        qDebug( "Updated" );
    dbclose();
}

void MainWindow::on_mod_book_clicked()
{
    bool ok = false;
        dbConnect();
       QString pseudo = QInputDialog::getText(this, "Stock", "donner le nouveau nombre d'exemplaires ?", QLineEdit::Normal, QString(), &ok);//lire le nouveau nombre de copies
       qDebug() << pseudo.toInt() ;
        qDebug() << id_book ;
        qDebug() << titre;
       QSqlQuery qry; //mettre à jour le nombre de copies
       qry.prepare("UPDATE Books SET Nombre_copie=:nb WHERE Titre='"+titre+"' ");
      qry.bindValue(":nb",pseudo.toInt());
       if( !qry.exec() )
          qDebug() << qry.lastError();
       else
          qDebug( "updated" );
       QSqlQuery qry1;
        QSqlQueryModel *model=new QSqlQueryModel();
       qry1.prepare("SELECT * FROM Books");
       qry1.exec();
       model->setQuery(qry1);
       ui->AdminBook->setModel(model);
       ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

    dbclose();

}
