#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>

class Product : public QObject
{
    Q_OBJECT
public:


    explicit Product(QObject *parent,QString ID, QString name,
            QString code, QString productionDate, QString expirationDate, int price, int stockQuantity);



    QString name() const;
    QString code() const;
    QString productionDate() const;
    QString expirationDate() const;
    int price() const;
    int stockQuantity() const;

signals:

public slots:

private:
    QString m_ID;
    QString m_name;
    QString m_code;
    QString m_productionDate;
    QString m_expirationDate;
    int m_price;
    int m_stockQuantity;
};



#endif // PRODUCT_H
