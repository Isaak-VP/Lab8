#ifndef FORMSTUDENTS_H
#define FORMSTUDENTS_H

#include <QWidget>
#include <QString>
#include "mainwindow.h"
#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>


namespace Ui {
class FormStudents;
}



class FormStudents : public QWidget
{
    Q_OBJECT

public:
    explicit FormStudents(QWidget *parent = nullptr);
    ~FormStudents();
    void setNU(const QString& userName);

private slots:
    void on_pushButton_clicked();
    void loadVotesToListView();
      void initializeStudentId();
    void on_pushButton_2_clicked();
    void loadUsersToTableView();
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::FormStudents *ui;
     QStringListModel *model;
    QString nu;
      int currentStudentId;
};

#endif // FORMSTUDENTS_H
