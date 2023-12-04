#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>

class Inventory : public QObject
{
    Q_OBJECT
public:
    explicit Inventory(QObject *parent,int id,QString name,QString address,int capacity);

    int ID() const;
    QString name() const;
    QString address() const;
    int capacity() const;

signals:

private:
    int m_ID;
    QString m_name;
    QString m_address;
    int m_capacity;

};

#endif // INVENTORY_H
