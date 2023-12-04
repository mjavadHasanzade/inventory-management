#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addproductdialog.h"
#include <QAbstractItemModel>

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


    // Close the database connection
    db.getDb().close();

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


