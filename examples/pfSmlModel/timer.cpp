#include "timer.h"

#include <QDebug>

#include <QTimer>

#include <Windows.h>
#include <iostream>

Timer::Timer(QObject *parent) : QObject(parent)
{
    g_pTimer = new QTimer(this);

    connect(g_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    g_ada = nullptr;
}
Timer::~Timer()
{
    //g_pTimer->stop();
}

void Timer::handleTimeout()
{
    double data = g_ada->value();
    std::cout << "Data(" << data << ")" << std::endl;
}

