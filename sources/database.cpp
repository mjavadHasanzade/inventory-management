#include "../headers/database.h"


Database::Database(QObject *parent)
    : QObject{parent}
{

}

QSqlDatabase Database::getDb() const
{
    return db;
}

void Database::executeQuery(QString q)
{
    db.open();
    QSqlQuery query;
    if (!query.exec(q)) {
        qWarning() << "Failed to execute insert query:" << query.lastError().text();
        return; // return invalid QSqlQuery
    }
    db.close();
}


void Database::setupDatabase()
{
    // Create and open the database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/tut_cpp/inventory-management/dev.db");
    if (!db.open()) {
        qDebug() << "Failed to open database:";
        qDebug() << db.lastError().text();
        return ;
    }else{
        emit dbConnected();
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS products ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name VARCHAR(50) NOT NULL,"
                    "code VARCHAR(20) NOT NULL,"
                    "production_date DATE,"
                    "expiration_date DATE,"
                    "price DECIMAL(10, 2) NOT NULL,"
                    "quantity_stock INT NOT NULL"
                    ")")) {
        qWarning() << "Failed to create 'products' table:" << query.lastError().text();
        return ;
    }

    // Create the "warehouses" table
    if (!query.exec("CREATE TABLE IF NOT EXISTS warehouses ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name VARCHAR(50) NOT NULL,"
                    "address VARCHAR(100) NOT NULL,"
                    "capacity INT NOT NULL,"
                    "capacityLeft INT NOT NULL"
                    ")")) {
        qWarning() << "Failed to create 'warehouses' table:" << query.lastError().text();
        return ;
    }

    // Create the "product_warehouse" table
    if (!query.exec("CREATE TABLE IF NOT EXISTS product_warehouse ("
                    "product_id INT,"
                    "warehouse_id INT,"
                    "quantity INT,"
                    "PRIMARY KEY (product_id, warehouse_id),"
                    "FOREIGN KEY (product_id) REFERENCES products(id),"
                    "FOREIGN KEY (warehouse_id) REFERENCES warehouses(id)"
                    ")")) {
        qWarning() << "Failed to create 'product_warehouse' table:" << query.lastError().text();
        return ;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS orders ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "product_id INT,"
                    "warehouse_id INT,"
                    "quantity INT,"
                    "FOREIGN KEY (product_id) REFERENCES products(id),"
                    "FOREIGN KEY (warehouse_id) REFERENCES warehouses(id)"
                    ")")) {
        qWarning() << "Failed to create 'orders' table:" << query.lastError().text();
        return ;
    }

    // Create the "transactions" table
    if (!query.exec("CREATE TABLE IF NOT EXISTS transactions ("
                    "product_id INT,"
                    "warehouse_id INT,"
                    "quantity INT,"
                    "FOREIGN KEY (product_id) REFERENCES products(id),"
                    "FOREIGN KEY (warehouse_id) REFERENCES warehouses(id),"
                    "PRIMARY KEY (product_id, warehouse_id)"
                    ")")) {
        qWarning() << "Failed to create 'transactions' table:" << query.lastError().text();
        return ;
    }
}
