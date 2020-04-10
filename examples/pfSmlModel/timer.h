#ifndef TIMER_H
#define TIMER_H

#include <QObject>

#include "ModelManager/modelmanager.h"
#include <QTimer>

class QTimer;

class Timer: public QObject
{
Q_OBJECT
public:
    Timer(QObject* parent = NULL);
    ~Timer();

public slots:
    void handleTimeout(); //超时处理函数

public:
    Algorithm *g_ada;
    QTimer *g_pTimer;
};

#endif // TIMER_H
