#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "./product.h"
#include <QVector>
#include <QMessageBox>


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

    //==========Storages======================================

    QList<Product*> products;


    void initilizeTableProducts();
    void updateProductsTable();

};

#endif // MAINWINDOW_H
