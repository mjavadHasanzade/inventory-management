#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addproductdialog.h"
#include <QAbstractItemModel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initilizeTableProducts();
    statusBar()->showMessage("   Ready...",3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initilizeTableProducts()
{
    productModel->setColumnCount(6);

    productModel->setHorizontalHeaderItem(0,new QStandardItem("Name"));
    productModel->setHorizontalHeaderItem(1,new QStandardItem("Code"));
    productModel->setHorizontalHeaderItem(2,new QStandardItem("Production Date"));
    productModel->setHorizontalHeaderItem(3,new QStandardItem("Expiration Date"));
    productModel->setHorizontalHeaderItem(4,new QStandardItem("Price"));
    productModel->setHorizontalHeaderItem(5,new QStandardItem("Quantity"));

    ui->tableViewProducts->setModel(productModel);

    Product *product0=new Product(this, "P123456", "T-Shirt", "TS123", "2022/05/15", "2023/05/14", 29.99, 50);
    Product *product1=new Product(this, "P987654", "Jeans", "JN789", "2022/08/01", "2024/08/01", 49.99, 100);
    Product *product2=new Product(this, "P567890", "Sneakers", "SN456", "2022/06/10", "2023/06/09", 79.99, 75);
    Product *product3=new Product(this, "P246810", "Backpack", "BP123", "2022/07/20", "2024/07/19", 39.99, 50);

    products<<product0<<product1<<product2<<product3;
    updateProductsTable();

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
            qDebug()<<products.at(row)->ID();
        }


    }else{
        QMessageBox::information(this,"Info","Please Select one row.");
    }

}

