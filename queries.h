#ifndef QUERIES_H
#define QUERIES_H

#include <QObject>

class Queries : public QObject
{
    Q_OBJECT
public:
    explicit Queries(QObject *parent = nullptr);
       QString insertProductQuery="INSERT INTO products (name, code, production_date, expiration_date, price, quantity_stock) VALUES ('%1', '%2', '%3', '%4', %5, %6)";
       QString deleteProductQuery="DELETE FROM products WHERE id = %1";
       QString updateProductQuery="UPDATE products SET name = '%1', code = '%2', production_date = '%3', expiration_date = '%4', price = '%5', quantity_stock = '%6' WHERE id = '%7'";
       QString getAllProductsQuery="SELECT * FROM products";

signals:

};

#endif // QUERIES_H
