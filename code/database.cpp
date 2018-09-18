#include "database.h"
#include <QDebug>
#include <QFileInfo>
<<<<<<< HEAD
#include <QDir>
=======
>>>>>>> 02f0af2404ecd88c2fa5245ae04ada9c5700d253


Database::Database()
{
<<<<<<< HEAD
     //path = "/home/hamma/Project/qt/Gestion_Bibliotheque/Gestion";
     path = "../Gestion_Bibliotheque/Gestion";
=======
    path = "/home/hamma/Project/qt/Gestion_Bibliotheque/Gestion";

>>>>>>> 02f0af2404ecd88c2fa5245ae04ada9c5700d253
}

bool Database::dbConnect(){ // connect to the base
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()){
        qDebug() << "error connection ";
        return false;
    }else{
        qDebug() << "connected ";
        return true;
    }
}

void Database::dbclose(){ // deconnect from the base

    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);

}
