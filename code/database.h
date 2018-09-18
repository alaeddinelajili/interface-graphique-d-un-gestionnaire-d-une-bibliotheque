#ifndef DATABASE_H
#define DATABASE_H
#include <QSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFile>
#include <QString>

class Database
{
private:
    QSqlDatabase db;
    QFile file;
    QString path;
public:
    Database();
    bool dbConnect();
    void dbclose();
};

#endif // DATABASE_H
