#ifndef USERS_H
#define USERS_H

#include <QWidget>

class Users : public QWidget
{

protected:
    QString nu;
public:

    explicit Users(QWidget *parent = nullptr);
    virtual ~Users();

    // Метод для встановлення імені користувача
    virtual void setNU(const QString &userName);
};

#endif // USERS_H
