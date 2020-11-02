#include <finaltable.h>
#include <ui_finaltable.h>

Finaltable::Finaltable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Finaltable)
{
    ui->setupUi(this);
    this->setWindowTitle("Tabla de procesos");
    prepareTable();
}

Finaltable::~Finaltable()
{
    delete ui;
}

void Finaltable::prepareTable()
{
    this->setGeometry(300,300,755,310);
    ui->finishedProcessesTable->setGeometry(10,10,738,290);
    QStringList header;
    header<<"ID"<<"TME"<<"Operación"<<"Resultado"<<"TLlegada"<<"TFinalización"<<"TRetorno"<<"TRespuesta"<<"TEspera"<<"TServicio";
    ui->finishedProcessesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->finishedProcessesTable->setColumnCount(10);
    ui->finishedProcessesTable->setHorizontalHeaderLabels(header);
    ui->finishedProcessesTable->verticalHeader()->hide();
    ui->finishedProcessesTable->setColumnWidth(0,35);
    ui->finishedProcessesTable->setColumnWidth(1,35);
    for (int i=2 ; i<10 ; i++ )
    {
        ui->finishedProcessesTable->setColumnWidth(i,80);
    }
    header.clear();
}

void Finaltable::finishedProcessesTable(processesList processes)
{
    int row = 0;
    node *aux = processes.first;
    while(aux)
    {
        ui->finishedProcessesTable->insertRow(row);
        ui->finishedProcessesTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID)));
        ui->finishedProcessesTable->setItem(row,1,new QTableWidgetItem(QString::number(aux->process.TME)));
        ui->finishedProcessesTable->setItem(row,2,new QTableWidgetItem(aux->process.operation));
        if(aux->process.status == 99) //ERROR = 99
        {
            ui->finishedProcessesTable->setItem(row,3,new QTableWidgetItem("ERROR"));
        }
        else
            ui->finishedProcessesTable->setItem(row,3,new QTableWidgetItem(QString::number(aux->process.result)));

        ui->finishedProcessesTable->setItem(row,4,new QTableWidgetItem(QString::number(aux->process.arrivalTime)));
        ui->finishedProcessesTable->setItem(row,5,new QTableWidgetItem(QString::number(aux->process.endTime)));
        ui->finishedProcessesTable->setItem(row,6,new QTableWidgetItem(QString::number(aux->process.returnTime)));
        ui->finishedProcessesTable->setItem(row,7,new QTableWidgetItem(QString::number(aux->process.responseTime)));
        ui->finishedProcessesTable->setItem(row,8,new QTableWidgetItem(QString::number(aux->process.waitTime)));
        ui->finishedProcessesTable->setItem(row,9,new QTableWidgetItem(QString::number(aux->process.serviceTime)));


        for (int column=0 ; column<10 ; column++)
        {
            ui->finishedProcessesTable->item(row,column)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
        aux = aux->next;
    }
    for (int i=0; i<=row; i++)
    {
        ui->finishedProcessesTable->removeRow(row);
    }
}

