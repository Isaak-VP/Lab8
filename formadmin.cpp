#include "formadmin.h"
#include "ui_formadmin.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QSqlQueryModel>
#include <QTimer>

FormAdmin::FormAdmin(QWidget *parent)
    : Users(parent)
    , ui(new Ui::FormAdmin)
{
    ui->setupUi(this);
     loadDataToListView();
    loadStudentsToListView();
     loadCandidatsToListView();
    loadVotesToListView();
}

FormAdmin::~FormAdmin()
{
    delete ui;
}

void FormAdmin::setNU(const QString &userName)
{
    Users::setNU(userName);
    ui->label->setText(userName);
}

void FormAdmin::on_pushButton_clicked()
{
    MainWindow* w = new MainWindow;
    w->show();

    this->close();
}

void FormAdmin::on_pushButton_2_clicked()
{


        QString nameGolos = ui->plainTextEdit->toPlainText();


        if (nameGolos.isEmpty()) {
            QMessageBox::warning(this, "Помилка", "Поле не може бути порожнім!");
            return;
        }


        QSqlQuery query;
        query.prepare("INSERT INTO Golos (NameGolos) VALUES (:nameGolos)");
        query.bindValue(":nameGolos", nameGolos);


        if (!query.exec()) {
            qDebug() << "Помилка вставки в таблицю Golos:" << query.lastError().text();
            QMessageBox::critical(this, "Помилка", "Не вдалося додати запис у базу даних.");
        } else {
            qDebug() << "Успішно додано запис у таблицю Golos:" << nameGolos;
            QMessageBox::information(this, "Успіх", "Запис успішно додано в базу даних!");


            ui->plainTextEdit->clear();
        }
    loadDataToListView();

}


void FormAdmin::on_pushButton_3_clicked()
{

    QModelIndex selectedIndex = ui->listView->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Помилка", "Виберіть об'яву для видалення.");
        return;
    }

    QString selectedName = selectedIndex.data().toString();


    QSqlQuery query;
    query.prepare("DELETE FROM Golos WHERE NameGolos = :nameGolos");
    query.bindValue(":nameGolos", selectedName);

    if (!query.exec()) {
        qDebug() << "Помилка видалення запису:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити об'яву.");
    } else {
        QMessageBox::information(this, "Успіх", "Об'яву успішно видалено.");
        loadDataToListView();
    }
    loadDataToListView();
}


void FormAdmin::on_pushButton_4_clicked()
{


    if (QMessageBox::question(this, "Підтвердження", "Ви впевнені, що хочете видалити всі об'яви?")
        == QMessageBox::No) {
        return;
    }


    QSqlQuery query;
    if (!query.exec("DELETE FROM Golos")) {
        qDebug() << "Помилка видалення всіх записів:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити всі об'яви.");
    } else {
        QMessageBox::information(this, "Успіх", "Всі об'яви успішно видалено.");
        loadDataToListView();
    }

}


void FormAdmin::loadDataToListView() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT NameGolos FROM Golos");

    if (model->lastError().isValid()) {
        qDebug() << "Помилка завантаження даних у listView:" << model->lastError().text();
        return;
    }

    ui->listView->setModel(model);
}


void FormAdmin::on_pushButton_5_clicked()
{

    QString userName = ui->lineEdit->text();
    QString login = ui->lineEdit_2->text();
    QString pass = ui->lineEdit_3->text();
    QString group = ui->lineEdit_4->text();

    if (userName.isEmpty() || login.isEmpty() || pass.isEmpty() || group.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Всі поля мають бути заповнені!");
        return;
    }

    QSqlQuery query;


    query.prepare("INSERT INTO Users (UserName, Login, Pass, Position) VALUES (:userName, :login, :pass, 'Студент')");
    query.bindValue(":userName", userName);
    query.bindValue(":login", login);
    query.bindValue(":pass", pass);

    if (!query.exec()) {
        qDebug() << "Помилка додавання в таблицю Users:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося зареєструвати користувача.");
        return;
    }


    int userId = query.lastInsertId().toInt();


    query.prepare("INSERT INTO Students (IdUser, `Group`) VALUES (:idUser, :group)");
    query.bindValue(":idUser", userId);
    query.bindValue(":group", group);

    if (!query.exec()) {
        qDebug() << "Помилка додавання в таблицю Students:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося зареєструвати студента.");
        return;
    }

    QMessageBox::information(this, "Успіх", "Користувача успішно зареєстровано!");


    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();


    loadStudentsToListView();

}


void FormAdmin::on_pushButton_6_clicked()
{

    QModelIndex selectedIndex = ui->listView_2->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Помилка", "Виберіть студента для видалення.");
        return;
    }


    int idUser = ui->listView_2->model()->index(selectedIndex.row(), 2).data().toInt();


    QSqlQuery query;
    query.prepare("DELETE FROM Students WHERE IdUser = :idUser");
    query.bindValue(":idUser", idUser);

    if (!query.exec()) {
        qDebug() << "Помилка видалення студента з таблиці Students:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити студента.");
        return;
    }

    query.prepare("DELETE FROM Users WHERE IdUser = :idUser");
    query.bindValue(":idUser", idUser);

    if (!query.exec()) {
        qDebug() << "Помилка видалення користувача з таблиці Users:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити користувача.");
        return;
    }

    QMessageBox::information(this, "Успіх", "Студента успішно видалено!");
    loadStudentsToListView();
    loadVotesToListView();

}


void FormAdmin::loadStudentsToListView() {
    QSqlQueryModel *model = new QSqlQueryModel(this);


    model->setQuery("SELECT Users.UserName, Students.`Group`, Students.IdUser FROM Students JOIN Users ON Students.IdUser = Users.IdUser");

    if (model->lastError().isValid()) {
        qDebug() << "Помилка завантаження даних у listView_2:" << model->lastError().text();
        return;
    }

    ui->listView_2->setModel(model);
    ui->listView_2->setModelColumn(0);
}



void FormAdmin::on_pushButton_7_clicked()
{

    QString userName = ui->lineEdit_8->text();


    if (userName.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Введіть ім'я користувача для перевірки.");
        return;
    }


    QSqlQuery query;
    query.prepare("SELECT IdUser FROM Users WHERE UserName = :userName");
    query.bindValue(":userName", userName);

    if (!query.exec()) {
        qDebug() << "Помилка перевірки UserName у таблиці Users:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося перевірити ім'я користувача.");
        return;
    }

    if (!query.next()) {

        QMessageBox::information(this, "Результат перевірки", "Користувача з таким ім'ям не знайдено в таблиці Users.");
        return;
    }


    int idUser = query.value(0).toInt();
    qDebug() << "IdUser знайдено в Users:" << idUser;


    query.prepare("SELECT IdCandidat FROM Candidats WHERE IdUser = :idUser");
    query.bindValue(":idUser", idUser);

    if (!query.exec()) {
        qDebug() << "Помилка перевірки IdUser у таблиці Candidats:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося перевірити, чи користувач є кандидатом.");
        return;
    }

    if (query.next()) {

        QMessageBox::information(this, "Результат перевірки", "Користувач уже є кандидатом у таблиці Candidats.");
    } else {

        query.prepare("INSERT INTO Candidats (IdUser, CountVoices) VALUES (:idUser, 0)");
        query.bindValue(":idUser", idUser);

        if (!query.exec()) {
            qDebug() << "Помилка додавання кандидата у таблицю Candidats:" << query.lastError().text();
            QMessageBox::critical(this, "Помилка", "Не вдалося додати користувача як кандидата.");
            return;
        }

        QMessageBox::information(this, "Успіх", "Користувача успішно додано до таблиці Candidats.");
    }


    ui->lineEdit_8->clear();


    loadCandidatsToListView();
    loadVotesToListView();
}


void FormAdmin::on_pushButton_8_clicked()
{
    QModelIndex selectedIndex = ui->listView_3->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Помилка", "Виберіть кандидата для видалення.");
        return;
    }


    int idUser = ui->listView_3->model()->index(selectedIndex.row(), 1).data().toInt();


    QSqlQuery query;
    query.prepare("DELETE FROM Candidats WHERE IdUser = :idUser");
    query.bindValue(":idUser", idUser);

    if (!query.exec()) {
        qDebug() << "Помилка видалення кандидата:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити кандидата.");
        return;
    }

    QMessageBox::information(this, "Успіх", "Кандидата успішно видалено!");


    loadCandidatsToListView();
    loadVotesToListView();
}



void FormAdmin::loadCandidatsToListView() {

    QSqlQueryModel *model = new QSqlQueryModel(this);


    model->setQuery("SELECT Users.UserName, Candidats.IdUser "
                    "FROM Candidats "
                    "JOIN Users ON Candidats.IdUser = Users.IdUser");


    if (model->lastError().isValid()) {
        qDebug() << "Помилка SQL-запиту:" << model->lastError().text();
        return;
    }


    if (model->rowCount() == 0) {
        qDebug() << "Дані відсутні у результатах запиту.";
        return;
    }


    ui->listView_3->setModel(model);
    ui->listView_3->setModelColumn(0);

    qDebug() << "Дані успішно завантажені в listView_3.";

}

void FormAdmin::loadVotesToListView() {
    QSqlQueryModel *model = new QSqlQueryModel(this);


    model->setQuery("SELECT Users.UserName AS `Ім'я`, Candidats.CountVoices AS `Голоси` "
                    "FROM Candidats "
                    "JOIN Users ON Candidats.IdUser = Users.IdUser");


    if (model->lastError().isValid()) {
        qDebug() << "Помилка завантаження даних у tableView:" << model->lastError().text();
        return;
    }


    ui->tableView->setModel(model);


    ui->tableView->setSortingEnabled(true);


    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, [=](int column, Qt::SortOrder order) {
        QString sortColumn = column == 0 ? "`Ім'я`" : "`Голоси`";
        QString orderText = order == Qt::AscendingOrder ? "ASC" : "DESC";


        model->setQuery(QString("SELECT Users.UserName AS `Ім'я`, Candidats.CountVoices AS `Голоси` "
                                "FROM Candidats "
                                "JOIN Users ON Candidats.IdUser = Users.IdUser "
                                "ORDER BY %1 %2").arg(sortColumn, orderText));


        if (model->lastError().isValid()) {
            qDebug() << "Помилка сортування даних:" << model->lastError().text();
        }
    });


    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void FormAdmin::on_pushButton_9_clicked()
{
    QSqlQuery query;


    query.prepare("UPDATE Students SET Voice = 1");

    if (!query.exec()) {
        qDebug() << "Помилка оновлення Voice у Students:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видати всім студентам голос.");
        return;
    }

    QMessageBox::information(this, "Успіх", "Усім студентам надано голос");
    ui->listView_3->setModel(nullptr);
    loadDataToListView();
    loadStudentsToListView();
    loadCandidatsToListView();
    loadVotesToListView();
}


void FormAdmin::on_pushButton_10_clicked()
{
    QSqlQuery query;


    query.prepare("UPDATE Students SET Voice = 0");

    if (!query.exec()) {
        qDebug() << "Помилка оновлення Voice у Students:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося забрати голоси у студентів.");
        return;
    }


    query.prepare("DELETE FROM Candidats");

    if (!query.exec()) {
        qDebug() << "Помилка видалення всіх записів у Candidats:" << query.lastError().text();
        QMessageBox::critical(this, "Помилка", "Не вдалося видалити кандидатів.");
        return;
    }

    QMessageBox::information(this, "Успіх", "Усім студентам встановлено кількість голосів 0, а всіх кандидатів видалено.");
    ui->listView_3->setModel(nullptr);
    loadDataToListView();
    loadStudentsToListView();
    loadCandidatsToListView();
    loadVotesToListView();
}

