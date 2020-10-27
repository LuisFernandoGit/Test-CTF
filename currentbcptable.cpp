#include "currentbcptable.h"
#include "ui_currentbcptable.h"

currentBCPTable::currentBCPTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::currentBCPTable)
{
    ui->setupUi(this);
    this->setWindowTitle("Tabla de procesos");
    prepareTable();
}

currentBCPTable::~currentBCPTable()
{
    delete ui;
}

void currentBCPTable::loadMemoryTable(processesList running, processesList ready, processesList blocked,processesList news ,processesList finished,processesList suspended)
{
    int row = 0;
    node *aux = running.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("PROCESANDO"));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        ui->BCPTable->setItem(row,3,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.serviceTime)));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.getTR())));
        ui->BCPTable->setItem(row,10,new QTableWidgetItem(QString::number(aux->process.responseTime)));
        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    aux = ready.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("LISTO"));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        ui->BCPTable->setItem(row,3,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.serviceTime)));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.getTR())));
        if(aux->process.responseTime != -1)
            ui->BCPTable->setItem(row,10,new QTableWidgetItem(QString::number(aux->process.responseTime)));
        else
            ui->BCPTable->setItem(row,10,new QTableWidgetItem("-"));
        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    aux = blocked.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("BLOQUEADO: " + QString::number(aux->process.blockedTime)));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        ui->BCPTable->setItem(row,3,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.serviceTime)));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.getTR())));
        ui->BCPTable->setItem(row,10,new QTableWidgetItem(QString::number(aux->process.responseTime)));

        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    aux = suspended.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("Suspendido: " + QString::number(aux->process.blockedTime)));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        ui->BCPTable->setItem(row,3,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.serviceTime)));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.getTR())));
        ui->BCPTable->setItem(row,10,new QTableWidgetItem(QString::number(aux->process.responseTime)));

        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    aux = news.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("NUEVO"));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        ui->BCPTable->setItem(row,3,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem("-"));
        ui->BCPTable->setItem(row,10,new QTableWidgetItem("-"));

        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    aux = finished.first;
    while(aux){
        ui->BCPTable->insertRow(row);
        ui->BCPTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->BCPTable->setItem(row,1,new QTableWidgetItem("FINALIZADO"));
        ui->BCPTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        if(aux->process.status == 99)
            ui->BCPTable->setItem(row,3,new QTableWidgetItem("ERROR"));
        else
            ui->BCPTable->setItem(row,3,new QTableWidgetItem(QString::number(aux->process.result)));
        ui->BCPTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->BCPTable->setItem(row,5,new QTableWidgetItem(QString::number(aux->process.endTime)));
        ui->BCPTable->setItem(row,6,new QTableWidgetItem(QString::number(aux->process.returnTime)));
        ui->BCPTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->BCPTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.serviceTime)));
        ui->BCPTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.getTR())));
        ui->BCPTable->setItem(row,10,new QTableWidgetItem(QString::number(aux->process.responseTime)));

        for (int column=0 ; column<11 ; column++)
                ui->BCPTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }

    for (int i=0; i<=row; i++)
        ui->BCPTable->removeRow(row);
}

void currentBCPTable::prepareTable()
{
    this->setGeometry(300,300,880,320);
    ui->BCPTable->setGeometry(10,10,860,300);

    QStringList header;
    header<<"ID"<<"Estado"<<"Operación"<<"Resultado"<<"TLlegada"<<"TFinalización"<<"TRetorno"<<"TEspera"<<"TServicio"<<"TRestante"<<"TRespuesta";
    ui->BCPTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BCPTable->setColumnCount(11);
    ui->BCPTable->setHorizontalHeaderLabels(header);
    ui->BCPTable->verticalHeader()->hide();
    ui->BCPTable->setColumnWidth(0,35);
    ui->BCPTable->setColumnWidth(1,90);
    for (int i=2 ; i<11 ; i++ )
        ui->BCPTable->setColumnWidth(i,80);
    header.clear();
}
