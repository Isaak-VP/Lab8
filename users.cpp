#include "users.h"



Users::Users(QWidget *parent)
    : QWidget(parent)
{
}

Users::~Users()
{
}

void Users::setNU(const QString &userName)
{
    nu = userName;
}
