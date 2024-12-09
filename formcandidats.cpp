#include "formcandidats.h"
#include "ui_formcandidats.h"
#include <QSqlQueryModel>
#include <QStringList>
#include <QTableView>
#include <QTimer>


FormCandidats::FormCandidats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormCandidats)
{
    ui->setupUi(this);
    loadVotesToListView();
}

FormCandidats::~FormCandidats()
{
    delete ui;
}

void FormCandidats::setNU(const QString &userName){
    ui->label->setText(userName);
    nu=userName;
}

void FormCandidats::setVoteCount(int count) {
    ui->voteLabel->setText(QString::number(count));
}


void FormCandidats::on_pushButton_clicked()
{
    MainWindow* w = new MainWindow;
    w->show();

    this->close();
}


void FormCandidats::loadVotesToListView() {
    QSqlQueryModel *model = new QSqlQueryModel(this);


    model->setQuery(
        "SELECT Users.UserName AS `Ім'я`, Students.Voice AS `Голоси` "
        "FROM Users "
        "INNER JOIN Students ON Users.IdUser = Students.IdUser"
        );


    if (model->lastError().isValid()) {
        qDebug() << "Помилка завантаження даних у tableView:" << model->lastError().text();
        return;
    }


    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);


    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, [=](int column, Qt::SortOrder order) {
        QString sortColumn = column == 0 ? "`Ім'я`" : "`Голоси`";
        QString orderText = order == Qt::AscendingOrder ? "ASC" : "DESC";


        model->setQuery(QString(
                            "SELECT Users.UserName AS `Ім'я`, Students.Voice AS `Голоси` "
                            "FROM Users "
                            "INNER JOIN Students ON Users.IdUser = Students.IdUser "
                            "ORDER BY %1 %2").arg(sortColumn, orderText));


        if (model->lastError().isValid()) {
            qDebug() << "Помилка сортування даних:" << model->lastError().text();
        }
    });


    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


