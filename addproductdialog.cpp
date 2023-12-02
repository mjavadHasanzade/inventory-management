#include "addproductdialog.h"
#include "ui_addproductdialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddProductDialog::AddProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProductDialog)
{
    ui->setupUi(this);
    setupRegex();
}

AddProductDialog::AddProductDialog(QWidget *parent, QString name, QString code, QString productionDate,
                                   QString expirationDate, int price, int stockQuantity)
    : QDialog(parent),ui(new Ui::AddProductDialog), m_name(name), m_code(code),
    m_productionDate(productionDate), m_expirationDate(expirationDate), m_price(price), m_stockQuantity(stockQuantity)
{
    ui->setupUi(this);
    setupRegex();

    ui->nameLineEdit->setText(name);
    ui->codeLineEdit->setText(code);
    ui->priceLineEdit->setText(QString::number(price));
    ui->productiondateEdit->setDate(QDate::fromString(productionDate, "yyyy/MM/dd"));
    ui->expirationDateEdit->setDate(QDate::fromString(expirationDate, "yyyy/MM/dd"));
    ui->quantityLineEdit->setText(QString::number(stockQuantity));
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

void AddProductDialog::setupRegex()
{
    QRegularExpression  regex("^[0-9]+$");
    QValidator *validator = new QRegularExpressionValidator(regex,this);
    ui->priceLineEdit->setValidator(validator);
    ui->quantityLineEdit->setValidator(validator);
}


void AddProductDialog::on_pushButton_clicked()
{
    reject();
}

