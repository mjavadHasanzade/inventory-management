#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "./product.h"
#include <QVector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* productModel = new QStandardItemModel();

    //==========Storages======================================

    QList<Product*> products;


    void initilizeTableProducts();

};

#endif // MAINWINDOW_H
