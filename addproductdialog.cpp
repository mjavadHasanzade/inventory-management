#include "addproductdialog.h"
#include "ui_addproductdialog.h"
#include <QMessageBox>

AddProductDialog::AddProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProductDialog)
{
    ui->setupUi(this);
}

AddProductDialog::~AddProductDialog()
{
    delete ui;
}

void AddProductDialog::on_pushButton_2_clicked()
{
    if (!validateField(ui->nameLineEdit->text(), "Name"))
        return;
    m_name=ui->nameLineEdit->text();

    if (!validateField(ui->codeLineEdit->text(), "Code"))
        return;
    m_code=ui->codeLineEdit->text();

    if (!validateField(ui->productiondateEdit->text(), "Production Date"))
        return;
    m_productionDate=ui->productiondateEdit->text();

    if (!validateField(ui->expirationDateEdit->text(), "Expiration Date"))
        return;
    m_expirationDate=ui->expirationDateEdit->text();


    if (!validateField(ui->priceLineEdit->text(), "Price"))
        return;
    m_price = ui->priceLineEdit->text().toInt();


    if (!validateField(ui->quantityLineEdit->text(), "Stock Quantity"))
        return;
    m_stockQuantity = ui->quantityLineEdit->text().toInt();

    accept();


}

bool AddProductDialog::validateField(const QString& value, const QString& fieldName) {
    if (value.isEmpty()) {
        QMessageBox::critical(this, "Error", fieldName + " cannot be empty");
        return false;
    }
    return true;
}


void AddProductDialog::on_pushButton_clicked()
{
    reject();
}

