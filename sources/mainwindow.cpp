#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "../headers/addproductdialog.h"
#include <QAbstractItemModel>
#include "../headers/inventorydialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&db,&Database::dbConnected,this,[=](){
        statusBar()->showMessage("Connectd to Database...",3000);
    });

    db.setupDatabase();
    initilizeTableProducts();
    initilizeTableInventories();

    db.getDb().close();
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
    if (db.getDb().isOpen()) {
        QSqlQuery query;
        if (query.exec(queries.getAllProductsQuery)) {
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

        QString q = queries.insertProductQuery.arg(addProductDialog->m_name,
                                                   addProductDialog->m_code, addProductDialog->m_productionDate,
                                                   addProductDialog->m_expirationDate, QString::number(addProductDialog->m_price),
                                                   QString::number(addProductDialog->m_stockQuantity));

        // Execute the insert query
        db.executeQuery(q);

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

            db.executeQuery(queries.deleteProductQuery.arg(products.at(row)->ID()));

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


            QString q = queries.updateProductQuery
                            .arg(addProductDialog->m_name,
                                 addProductDialog->m_code, addProductDialog->m_productionDate,
                                 addProductDialog->m_expirationDate, QString::number(addProductDialog->m_price),
                                 QString::number(addProductDialog->m_stockQuantity),products.at(row)->ID());
            db.executeQuery(q);

            products.replace(row,editedProduct);
            updateProductsTable();
        }


    }else{
        QMessageBox::information(this,"Info","Please Select one row.");
    }

}


//===================================> Inventories <==============================================================//



void MainWindow::initilizeTableInventories()
{
    inventoryModel->setColumnCount(4);

    inventoryModel->setHorizontalHeaderItem(0,new QStandardItem("Name"));
    inventoryModel->setHorizontalHeaderItem(1,new QStandardItem("Address"));
    inventoryModel->setHorizontalHeaderItem(2,new QStandardItem("Capacity"));
    inventoryModel->setHorizontalHeaderItem(3,new QStandardItem("Capacity Left"));

    ui->inventoriesTableView->setModel(inventoryModel);

    // Perform database operations
    if (db.getDb().isOpen()) {
        QSqlQuery query;
        if (query.exec(queries.getAllInventoriesQuery)) {
            // Process the query results
            while (query.next()) {
                Inventory *inv=new Inventory(this, query.value("id").toInt(), query.value("name").toString()
                                               ,query.value("address").toString(),query.value("capacity").toInt()
                                               ,query.value("capacityLeft").toInt());
                inventories.append(inv);

            }
            updateInventoriesTable();
        } else {
            qDebug() << "Failed to execute query:";
            qDebug() << query.lastError().text();
        }
    }


    // Close the database connection
    db.getDb().close();
}

void MainWindow::updateInventoriesTable()
{
    QAbstractItemModel* model =  ui->inventoriesTableView->model();
    if(model){
        model->removeRows(0, model->rowCount());
    }

    for (int i = 0; i < inventories.length(); ++i) {
        int newRow = inventoryModel->rowCount();
        inventoryModel->setItem(newRow, 0, new QStandardItem(inventories.at(i)->name()));
        inventoryModel->setItem(newRow, 1, new QStandardItem(inventories.at(i)->address()));
        inventoryModel->setItem(newRow, 2, new QStandardItem(QString::number(inventories.at(i)->capacity())));
        inventoryModel->setItem(newRow, 3, new QStandardItem(QString::number(inventories.at(i)->capacityLeft())));
    }
}


void MainWindow::on_actionInventory_triggered()
{
    InventoryDialog *invDialog=new InventoryDialog(this);

    int ret= invDialog->exec();

    ui->tabWidget->setCurrentIndex(1);

    if(ret == QDialog::Accepted){
        Inventory *newInv=new Inventory(this,0,invDialog->name(),invDialog->address(),invDialog->capacity(),invDialog->capacity());

        QString q = queries.insertInventoryQuery.arg(invDialog->name(), invDialog->address(),
                                                     QString::number(invDialog->capacity()), QString::number(invDialog->capacity()));
        db.executeQuery(q);

        inventories.append(newInv);
        updateInventoriesTable();
    }
}


void MainWindow::on_deleteInventoryBtn_clicked()
{
    QModelIndexList selectedRows = ui->inventoriesTableView->selectionModel()->selectedRows();

    // Check if exactly one row is selected
    if (selectedRows.size() == 1) {

        int ret = QMessageBox::question(this, "Confirmation", "Are you sure you want to proceed?",
                                        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

        if(ret==QMessageBox::Ok){

            int row = selectedRows.at(0).row();


            //no updating list
            //productModel->removeRow(row);
            db.getDb().open();
            QSqlQuery query;

            if(query.exec(queries.checkProductInWarehouseExistsQuery.arg(inventories.at(row)->ID()))){
                if(query.next()){
                    QMessageBox::critical(this,"Error","Inventory has Products");

                }else{

                    db.executeQuery(queries.deleteInventoryQuery.arg(inventories.at(row)->ID()));
                    inventories.removeAt(row);
                    updateInventoriesTable();
                }
            }


        }

    }else{
        QMessageBox::information(this,"Info","Please Select one row.");
    }
}

