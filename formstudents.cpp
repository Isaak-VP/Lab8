#include "formstudents.h"
#include "ui_formstudents.h"
#include <QTimer>

FormStudents::FormStudents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStudents)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    loadVotesToListView();
    loadUsersToTableView();
}

FormStudents::~FormStudents()
{
    delete ui;
}

void FormStudents::setNU(const QString &userName) {
    ui->label->setText(userName);
    nu = userName;
    initializeStudentId();
}

void FormStudents::on_pushButton_clicked()
{
    MainWindow* w = new MainWindow;
    w->show();

    this->close();
}



void FormStudents::initializeStudentId() {
    QSqlQuery query;
    query.prepare("SELECT IdStudent FROM Students JOIN Users ON Students.IdUser = Users.IdUser WHERE Users.UserName = :userName");
    query.bindValue(":userName", nu);

    if (!query.exec() || !query.next()) {
        qDebug() << "Не вдалося знайти IdStudent для поточного користувача:" << query.lastError().text();
        currentStudentId = -1;
        return;
    }

    currentStudentId = query.value(0).toInt();
    qDebug() << "Поточний студент IdStudent:" << currentStudentId;
}







void FormStudents::loadVotesToListView()
{

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qDebug() << "З'єднання з базою даних не відкрите:" << db.lastError().text();
        return;
    }


    QSqlQuery query(db);
    query.prepare("SELECT NameGolos FROM Golos");

    if (!query.exec()) {
        qDebug() << "Не вдалося виконати запит:" << query.lastError().text();
        return;
    }


    QStringList nameGolosList;


    while (query.next()) {
        QString nameGolos = query.value(0).toString();
        nameGolosList.append(nameGolos);
    }


    model->setStringList(nameGolosList);


    ui->listView->setModel(model);
}



void FormStudents::loadUsersToTableView()
{
    QSqlQuery query;
    query.prepare(
        "SELECT Users.IdUser, Users.UserName, Candidats.CountVoices "
        "FROM Users "
        "INNER JOIN Candidats ON Users.IdUser = Candidats.IdUser"
        );

    if (!query.exec()) {
        qDebug() << "Не вдалося виконати запит для tableView:" << query.lastError().text();
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Ім'я користувача");
    model->setHeaderData(2, Qt::Horizontal, "Кількість голосів");

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);

    // Розтягування колонок
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}




void FormStudents::on_pushButton_2_clicked()
{

    if (currentStudentId == -1) {
        QMessageBox::warning(this, "Помилка", "Неможливо знайти ваш профіль у базі даних.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "З'єднання з базою даних не відкрите:" << db.lastError().text();
        return;
    }

    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, виберіть кандидата для голосування.");
        return;
    }

    int selectedUserId = index.sibling(index.row(), 0).data().toInt();
    qDebug() << "Обраний кандидат IdUser:" << selectedUserId;

    QSqlQuery query(db);
    db.transaction();


    query.prepare("SELECT Voice FROM Students WHERE IdStudent = :idStudent");
    query.bindValue(":idStudent", currentStudentId);

    if (!query.exec() || !query.next()) {
        db.rollback();
        QMessageBox::warning(this, "Помилка", "Не вдалося отримати доступні голоси студента.");
        return;
    }

    int currentVoice = query.value(0).toInt();
    if (currentVoice <= 0) {
        db.rollback();
        QMessageBox::warning(this, "Помилка", "У вас більше немає голосів.");
        return;
    }


    query.prepare("UPDATE Students SET Voice = Voice - 1 WHERE IdStudent = :idStudent");
    query.bindValue(":idStudent", currentStudentId);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Помилка", "Не вдалося зменшити кількість голосів студента.");
        return;
    }


    query.prepare("UPDATE Candidats SET CountVoices = CountVoices + 1 WHERE IdUser = :idUser");
    query.bindValue(":idUser", selectedUserId);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Помилка", "Не вдалося збільшити кількість голосів у кандидата.");
        return;
    }

    db.commit();
    loadUsersToTableView();

    QMessageBox::information(this, "Успіх", "Ваш голос успішно віддано!");
}



void FormStudents::on_tableView_clicked(const QModelIndex &index)
{

}
