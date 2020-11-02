#include <proceso.h>

Proceso::Proceso()
{

}

void Proceso::new_process(int ID)
{
    this->ID = ID;
    this->TME = qrand() % 10 + 5;
    this->weight = qrand() % 23 + 8;

    int no1 = qrand() % 10;
    int no2 = qrand() % 10;
    switch (qrand() % 7)
    {
    case 0: // +
        result = no1 + no2;
        operation = QString::number(no1) + " + " + QString::number(no2);
        break;
    case 1: // -
        result = no1 - no2;
        operation = QString::number(no1) + " - " + QString::number(no2);
        break;
    case 2: // /
        result = ( no1 + 1 ) / ( no2 + 1 );
        operation = QString::number(no1 + 1) + " / " + QString::number(no2 + 1);
        break;
    case 3: // *
        result = no1 * no2;
        operation = QString::number(no1) + " * " + QString::number(no2);
        break;
    case 4: // %
        result = ( no1 + 1 ) % ( no2 + 1 );
        operation = QString::number(no1 +1) + " mod " + QString::number(no2 + 1);
        break;
    case 5: // r
        result = std::sqrt(no1+1);
        operation = QString::number(no1+1) + "sqr";
        break;
    case 6: // p
        result = no1 * no2 / 100; //no1 porciento de no2, ejem 10 porciento de 100 = 10
        operation = QString::number(no1) + " % " + QString::number(no2);
        break;
    default:
        result = no1 + no2;
        operation = QString::number(no1) + " + " + QString::number(no2);
    }

    serviceTime = 0;
    waitTime = 0;
    responseTime = -1;
    responseTimeFlag = false;
    status = 0; //NUEVO
}

int Proceso::getTR()
{
    return TME - serviceTime;
}

void Proceso::setBlockedTime(int time)
{
    blockedTime = time;
}
