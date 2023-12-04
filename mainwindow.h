#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "./product.h"
#include <QVector>
#include <QMessageBox>
#include "database.h"
#include "queries.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionProduct_triggered();

    void on_deleteProductBtn_clicked();

    void on_editProductBtn_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* productModel = new QStandardItemModel();
    Database db;
    Queries queries;

    //==========Storages======================================

    QList<Product*> products;

    //===========Functions====================================
    void setupDatabase();
    void initilizeTableProducts();
    void updateProductsTable();

};

#endif // MAINWINDOW_H
