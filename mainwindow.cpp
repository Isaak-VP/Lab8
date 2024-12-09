#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formadmin.h"
#include "formstudents.h"
#include "formcandidats.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectToDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("try2");
    db.setUserName("root");
    db.setPassword("root");

    if(!db.open()){
        qDebug()<< "Error connect to BD"<< db.lastError().text();

    } else {qDebug()<<"All good";}

}






bool MainWindow::authorizeUser(const QString& login, const QString& password) {
    QString choice = ui->comboBox->currentText();

    QSqlQuery userQuery;
    userQuery.prepare("SELECT IdUser, UserName, Position FROM Users WHERE Login = :login AND Pass = :password");
    userQuery.bindValue(":login", login);
    userQuery.bindValue(":password", password);

    if (!userQuery.exec()) {
        qDebug() << "ERROR Query Users:" << userQuery.lastError().text();
        return false;
    }

    if (userQuery.next()) {
        int userId = userQuery.value(0).toInt();
        QString userName = userQuery.value(1).toString();
        QString position = userQuery.value(2).toString();
        qDebug() << "User found in Users table:" << userName;


        if (choice == "Адміністратор" && position == "Адміністратор") {
            QSqlQuery adminQuery;
            adminQuery.prepare("SELECT IdAdmin FROM Admins WHERE IdUser = :idUser");
            adminQuery.bindValue(":idUser", userId);

            if (!adminQuery.exec()) {
                qDebug() << "ERROR Query Admins:" << adminQuery.lastError().text();
                return false;
            }

            if (adminQuery.next()) {

                qDebug() << "Good entry as Admin:" << userName;
                FormAdmin *adminForm = new FormAdmin();
                adminForm->setNU(userName);
                adminForm->show();
                this->hide();
                return true;
            } else {
                qDebug() << "User not found in Admins table.";
                return false;
            }
        }


        else if (choice == "Учень" && position == "Студент") {
            QSqlQuery studentQuery;
            studentQuery.prepare("SELECT IdStudent FROM Students WHERE IdUser = :idUser");
            studentQuery.bindValue(":idUser", userId);

            if (!studentQuery.exec()) {
                qDebug() << "ERROR Query Students:" << studentQuery.lastError().text();
                return false;
            }

            if (studentQuery.next()) {

                qDebug() << "Good entry as Student:" << userName;
                FormStudents *studentsForm = new FormStudents();
                studentsForm->setNU(userName);
                studentsForm->show();
                this->hide();
                return true;
            } else {
                qDebug() << "User not found in Students table.";
                return false;
            }
        }


        else if (choice == "Кандидат" && position == "Студент") {
            QSqlQuery candidatQuery;
            candidatQuery.prepare("SELECT IdCandidat, CountVoices FROM Candidats WHERE IdUser = :idUser");
            candidatQuery.bindValue(":idUser", userId);

            if (!candidatQuery.exec()) {
                qDebug() << "ERROR Query Candidats:" << candidatQuery.lastError().text();
                return false;
            }

            if (candidatQuery.next()) {
                int idCandidat = candidatQuery.value(0).toInt();
                int countVoices = candidatQuery.value(1).toInt();


                qDebug() << "Good entry as Candidat:" << userName << "with votes:" << countVoices;
                FormCandidats *candidatForm = new FormCandidats();
                candidatForm->setNU(userName);
                candidatForm->setVoteCount(countVoices);
                candidatForm->show();
                this->hide();
                return true;
            } else {
                qDebug() << "User not found in Candidats table.";
                return false;
            }
        } else {
            qDebug() << "Role mismatch for user:" << userName;
            return false;
        }
    } else {
        qDebug() << "User not found in Users table.";
        return false;
    }

    return false;
}




void MainWindow::on_pushButton_clicked()
{
    QString login = ui->lineEdit->text();
     QString password = ui->lineEdit_2->text();

    if (authorizeUser(login,password)){
         qDebug()<<"Succesful!!1";


    }else {
        QMessageBox::warning(this, "Ви помилились", "Не вірний логін або пароль");

    }
}

