#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>

namespace Ui {
class InventoryDialog;
}

class InventoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryDialog(QWidget *parent = nullptr);
    InventoryDialog(QWidget *parent,QString name, QString address, int capacity);
    ~InventoryDialog();

    QString name() const;
    QString address() const;
    int capacity() const;

private slots:
    void on_submitPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::InventoryDialog *ui;
    QString m_name;
    QString m_address;
    int m_capacity;

    bool validateField(const QString &value, const QString &fieldName);
    void setupRegex();
};

#endif // INVENTORYDIALOG_H
