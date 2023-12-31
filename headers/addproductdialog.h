#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class AddProductDialog;
}

class AddProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProductDialog(QWidget *parent = nullptr);
    AddProductDialog(QWidget *parent, QString name,
                     QString code, QString productionDate, QString expirationDate, int price, int stockQuantity);
    ~AddProductDialog();

    QString m_name;
    QString m_code;
    QString m_productionDate;
    QString m_expirationDate;
    int m_price;
    int m_stockQuantity;


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::AddProductDialog *ui;

    bool validateField(const QString& value, const QString& fieldName);
    void setupRegex();
};

#endif // ADDPRODUCTDIALOG_H
