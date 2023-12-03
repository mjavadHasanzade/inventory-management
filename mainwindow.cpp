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
    ui->tableViewProducts->verticalHeader()->setVisible(false);

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
                Product *product=new Product(this, "P123456", query.value("name").toString(), query.value("code").toString(),
                                               query.value("production_date").toString(), query.value("expiration_date").toString(),
                                               query.value("price").toInt(),query.value("quantity").toInt() );
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

            products.removeAt(row);
            updateProductsTable();

            //no updating list
            //productModel->removeRow(row);
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

    // Create the "products" table if it doesn't exist
    QSqlQuery createTableQuery;
    QString createTableSQL = "CREATE TABLE IF NOT EXISTS products ("
                             "id INTEGER PRIMARY KEY,"
                             "name TEXT,"
                             "code TEXT,"
                             "production_date DATE,"
                             "expiration_date DATE,"
                             "price REAL,"
                             "stock INTEGER,"
                             "quantity INTEGER"
                             ")";
    if (!createTableQuery.exec(createTableSQL)) {
        qDebug() << "Failed to create table:";
        qDebug() << createTableQuery.lastError().text();
        db.close();
        return ;
    }

}

