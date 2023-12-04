#include "../headers/product.h"

Product::Product(QObject *parent,QString ID, QString name, QString code, QString productionDate, QString expirationDate, int price, int stockQuantity)
    : QObject{parent},m_ID(ID), m_name(name), m_code(code), m_productionDate(productionDate), m_expirationDate(expirationDate), m_price(price), m_stockQuantity(stockQuantity)
{

}

QString Product::name() const
{
    return m_name;
}

QString Product::code() const
{
    return m_code;
}

QString Product::productionDate() const
{
    return m_productionDate;
}

QString Product::expirationDate() const
{
    return m_expirationDate;
}

int Product::price() const
{
    return m_price;
}

int Product::stockQuantity() const
{
    return m_stockQuantity;
}

QString Product::ID() const
{
    return m_ID;
}
