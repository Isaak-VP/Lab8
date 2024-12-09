#ifndef FORMCANDIDATS_H
#define FORMCANDIDATS_H

#include <QWidget>
#include <QString>
#include "mainwindow.h"


namespace Ui {
class FormCandidats;
}

class FormCandidats : public QWidget
{
    Q_OBJECT

public:
    explicit FormCandidats(QWidget *parent = nullptr);
    ~FormCandidats();
    void setNU(const QString& userName);
    void setVoteCount(int count);


private slots:
    void on_pushButton_clicked();
    void loadVotesToListView();

private:
    Ui::FormCandidats *ui;
    QString nu;
};

#endif // FORMCANDIDATS_H
