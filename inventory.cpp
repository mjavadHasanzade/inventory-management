#include "inventory.h"

Inventory::Inventory(QObject *parent, int id, QString name, QString address, int capacity)
    : QObject{parent},m_ID(id),m_name(name),m_address(address),m_capacity(capacity)
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
