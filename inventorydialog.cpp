#include "inventorydialog.h"
#include "ui_inventorydialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

InventoryDialog::InventoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);
    setupRegex();
}

InventoryDialog::InventoryDialog(QWidget *parent, QString name, QString address, int capacity):
    QDialog(parent),
    ui(new Ui::InventoryDialog),
    m_name(name),m_address(address),m_capacity(capacity)
{
    ui->setupUi(this);
    setupRegex();

}

InventoryDialog::~InventoryDialog()
{
    delete ui;
}

void InventoryDialog::on_submitPushButton_clicked()
{
    if (!validateField(ui->nameLineEdit->text(), "Name"))
        return;
    m_name=ui->nameLineEdit->text();

    if (!validateField(ui->addressLineEdit->text(), "Address"))
        return;
    m_address=ui->addressLineEdit->text();

    if (!validateField(ui->capacityLineEdit->text(), "Capacity"))
        return;
    m_capacity = ui->capacityLineEdit->text().toInt();

    accept();
}

QString InventoryDialog::name() const
{
    return m_name;
}

QString InventoryDialog::address() const
{
    return m_address;
}

int InventoryDialog::capacity() const
{
    return m_capacity;
}

bool InventoryDialog::validateField(const QString& value, const QString& fieldName) {
    if (value.isEmpty()) {
        QMessageBox::critical(this, "Error", fieldName + " cannot be empty");
        return false;
    }
    return true;
}

void InventoryDialog::setupRegex()
{
    QRegularExpression  regex("^[0-9]+$");
    QValidator *validator = new QRegularExpressionValidator(regex,this);
    ui->capacityLineEdit->setValidator(validator);

}


void InventoryDialog::on_cancelPushButton_clicked()
{
    reject();
}

