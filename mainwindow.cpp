#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addproductdialog.h"
#include <QAbstractItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setupDatabase();

    initilizeTableProducts();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initilizeTableProducts()
{
    //    ui->tableViewProducts->verticalHeader()->setVisible(false);
    productModel->setColumnCount(6);

    productModel->setHorizontalHeaderItem(0,new QStandardItem("Name"));
    productModel->setHorizontalHeaderItem(1,new QStandardItem("Code"));
    productModel->setHorizontalHeaderItem(2,new QStandardItem("Production Date"));
    productModel->setHorizontalHeaderItem(3,new QStandardItem("Expiration Date"));
    productModel->setHorizontalHeaderItem(4,new QStandardItem("Price"));
    productModel->setHorizontalHeaderItem(5,new QStandardItem("Quantity"));

    ui->tableViewProducts->setModel(productModel);

    // Perform database operations
    if (db.isOpen()) {
        // Example: Execute a query
        QSqlQuery query;
        if (query.exec("SELECT * FROM products")) {
            // Process the query results

            while (query.next()) {
                Product *product=new Product(this, query.value("id").toString(), query.value("name").toString(), query.value("code").toString(),
                                               query.value("production_date").toString(), query.value("expiration_date").toString(),
                                               query.value("price").toInt(),query.value("quantity_stock").toInt() );
                products.append(product);

            }
            updateProductsTable();
        } else {
            qDebug() << "Failed to execute query:";
            qDebug() << query.lastError().text();
        }
    }


    // Close the database connection
    db.close();

}

void MainWindow::updateProductsTable()
{
    QAbstractItemModel* model =  ui->tableViewProducts->model();
    if(model){
        model->removeRows(0, model->rowCount());
    }

    for (int i = 0; i < products.length(); ++i) {
        int newRow = productModel->rowCount();
        productModel->setItem(newRow, 0, new QStandardItem(products.at(i)->name()));
        productModel->setItem(newRow, 1, new QStandardItem(products.at(i)->code()));
        productModel->setItem(newRow, 2, new QStandardItem(products.at(i)->productionDate()));
        productModel->setItem(newRow, 3, new QStandardItem(products.at(i)->expirationDate()));
        productModel->setItem(newRow, 4, new QStandardItem(QString::number(products.at(i)->price())));
        productModel->setItem(newRow, 5, new QStandardItem(QString::number(products.at(i)->stockQuantity())));
    }
}


void MainWindow::on_actionProduct_triggered()
{
    AddProductDialog *addProductDialog=new AddProductDialog(this);
    int ret= addProductDialog->exec();

    if(ret==QDialog::Accepted){
        Product *newProduct=new Product(this,"P246810",addProductDialog->m_name,
                                          addProductDialog->m_code,addProductDialog->m_productionDate,addProductDialog->m_expirationDate
                                          ,addProductDialog->m_price,addProductDialog->m_stockQuantity);

        db.open();

        QString q = QString("INSERT INTO products (name, code, production_date, expiration_date, price, quantity_stock) "
                            "VALUES ('%1', '%2', '%3', '%4', %5, %6)").arg(addProductDialog->m_name,
                             addProductDialog->m_code, addProductDialog->m_productionDate,
                             addProductDialog->m_expirationDate, QString::number(addProductDialog->m_price),
                             QString::number(addProductDialog->m_stockQuantity));

        // Execute the insert query
        QSqlQuery query;
        if (!query.exec(q)) {
            qWarning() << "Failed to execute insert query:" << query.lastError().text();
            return ;
        }

        db.close();

        products.append(newProduct);
        updateProductsTable();
    }
}




void MainWindow::on_deleteProductBtn_clicked()
{
    QModelIndexList selectedRows = ui->tableViewProducts->selectionModel()->selectedRows();

    // Check if exactly one row is selected
    if (selectedRows.size() == 1) {

        int ret = QMessageBox::question(this, "Confirmation", "Are you sure you want to proceed?",
                                        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

        if(ret==QMessageBox::Ok){

            int row = selectedRows.at(0).row();


            //no updating list
            //productModel->removeRow(row);

            db.open();

            QSqlQuery delQuery;
            if (!delQuery.exec("DELETE FROM products WHERE id = "+products.at(row)->ID())) {
                qWarning() << "Failed to delete rows from 'products' table:" << delQuery.lastError().text();
                return;
            }
            db.close();

            products.removeAt(row);
            updateProductsTable();

        }

    }else{
        QMessageBox::information(this,"Info","Please Select one row.");
    }
}


void MainWindow::on_editProductBtn_clicked()
{

    QModelIndexList selectedRows = ui->tableViewProducts->selectionModel()->selectedRows();

    // Check if exactly one row is selected
    if (selectedRows.size() == 1) {

        int row = selectedRows.at(0).row();

        AddProductDialog *addProductDialog=new AddProductDialog(this,products.at(row)->name(),
                                                                  products.at(row)->code(),
                                                                  products.at(row)->productionDate(),
                                                                  products.at(row)->expirationDate(),
                                                                  products.at(row)->price(),
                                                                  products.at(row)->stockQuantity());

        int ret= addProductDialog->exec();

        if(ret==QDialog::Accepted){
            Product *editedProduct=new Product(this,products.at(row)->ID(),addProductDialog->m_name,
                                                 addProductDialog->m_code,addProductDialog->m_productionDate,addProductDialog->m_expirationDate
                                                 ,addProductDialog->m_price,addProductDialog->m_stockQuantity);

            db.open();

            QSqlQuery query;
            QString q = QString("UPDATE products SET name = '%1', code = '%2', production_date = '%3', expiration_date = '%4',"
                                " price = '%5', quantity_stock = '%6' WHERE id = '%7'")
                            .arg(addProductDialog->m_name,
                                 addProductDialog->m_code, addProductDialog->m_productionDate,
                                 addProductDialog->m_expirationDate, QString::number(addProductDialog->m_price),
                                 QString::number(addProductDialog->m_stockQuantity),products.at(row)->ID());

            if (!query.exec(q)) {
                qWarning() << "Failed to update rows in 'products' table:" << query.lastError().text();
                return ;
            }

            db.close();

            products.replace(row,editedProduct);
            updateProductsTable();
        }


    }else{
        QMessageBox::information(this,"Info","Please Select one row.");
    }

}

void MainWindow::setupDatabase()
{



    // Create and open the database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/tut_cpp/inventory-management/dev.db");
    if (!db.open()) {
        qDebug() << "Failed to open database:";
        qDebug() << db.lastError().text();
        return ;
    }else{
        statusBar()->showMessage("   Connectd to Database...",3000);
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
                    "capacity INT NOT NULL"
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

