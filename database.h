#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);


    void setupDatabase();
    QSqlDatabase getDb() const;
    void executeQuery(QString q);


signals:
    void dbConnected();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
