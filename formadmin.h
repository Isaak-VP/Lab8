#ifndef FORMADMIN_H
#define FORMADMIN_H

#include "users.h"
#include <QWidget>

namespace Ui {
class FormAdmin;
}

class FormAdmin : public Users
{
    Q_OBJECT

public:
    explicit FormAdmin(QWidget *parent = nullptr);
    ~FormAdmin();

    // Перевизначення методу setNU
    void setNU(const QString &userName) override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void loadDataToListView();

    void on_pushButton_5_clicked();
    void loadStudentsToListView();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();
    void loadCandidatsToListView();
    void loadVotesToListView();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::FormAdmin *ui;
};

#endif // FORMADMIN_H
