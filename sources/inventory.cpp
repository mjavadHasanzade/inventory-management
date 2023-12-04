#include "../headers/inventory.h"

Inventory::Inventory(QObject *parent, int id, QString name, QString address, int capacity,int capacityLeft)
    : QObject{parent},m_ID(id),m_name(name),m_address(address),m_capacity(capacity),m_capacityLeft(capacityLeft)
{

}

int Inventory::ID() const
{
    return m_ID;
}

QString Inventory::name() const
{
    return m_name;
}

QString Inventory::address() const
{
    return m_address;
}

int Inventory::capacity() const
{
    return m_capacity;
}

int Inventory::capacityLeft() const
{
    return m_capacityLeft;
}
