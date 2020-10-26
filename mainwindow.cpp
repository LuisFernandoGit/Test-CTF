#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include "node.h"
#include <QDebug>
#include "datafill.h"
#include <QMessageBox>
#include <QTime>
#include <fstream>

#define FINISHED 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define MAXPROCESS 4
#define PAUSED 5
#define BLOCKINGTIME 9
#define ERROR 99

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    ui->setupUi(this);
    ui->lcdNumber->setPalette(Qt::black);
    ui->lcdNumber_2->setPalette(Qt::black);
    ui->lcdNumber_3->setPalette(Qt::black);
    finalTableWindow = new finaltable(this);
    currentBCP = new currentBCPTable(this);
    prepare_inputs();
    this->setWindowTitle("Act 15");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    remove("suspendidos.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepare_inputs()
{
    status = 9999;
    cont = 0;
    newID = 1;
    quantum = 1;
    news.setUp();
    running.setUp();
    ready.setUp();
    blocked.setUp();
    finishedProcesses.setUp();

    ui->pushButtonStart->setEnabled(false);

    QStringList header;

    header<<"ID"<<"TME"<<"TR";
    ui->readyProcessesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->readyProcessesTable->setColumnCount(3);
    ui->readyProcessesTable->setHorizontalHeaderLabels(header);
    ui->readyProcessesTable->setColumnWidth(0,35);  //ID
    ui->readyProcessesTable->setColumnWidth(1,35);  //TME
    ui->readyProcessesTable->setColumnWidth(2,35);  //TT
    ui->readyProcessesTable->verticalHeader()->hide();
    header.clear();

    header<<"ID"<<"TME"<<"TR"<<"TBL";
    ui->blockedProcessesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->blockedProcessesTable->setColumnCount(4);
    ui->blockedProcessesTable->setHorizontalHeaderLabels(header);
    ui->blockedProcessesTable->setColumnWidth(0,35);  //ID
    ui->blockedProcessesTable->setColumnWidth(1,35);  //TME
    ui->blockedProcessesTable->setColumnWidth(2,35);  //TT
    ui->blockedProcessesTable->setColumnWidth(3,35);  //TT
    ui->blockedProcessesTable->verticalHeader()->hide();
    header.clear();

    header<<"ID"<<"OPERACION"<<"RESULTADO";
    ui->finishedProcessesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->finishedProcessesTable->setColumnCount(3);
    ui->finishedProcessesTable->setHorizontalHeaderLabels(header);
    ui->finishedProcessesTable->setColumnWidth(0,35); //ID
    //ui->finishedProcessesTable->setColumnWidth(1,50); //OPE
    //ui->finishedProcessesTable->setColumnWidth(2,60); //RESULT
    ui->finishedProcessesTable->verticalHeader()->hide();
    header.clear();

    ui->actionContinuar->setEnabled(false);
    ui->actionError->setEnabled(false);
    ui->actionInterrupci_n->setEnabled(false);
    ui->actionPausa->setEnabled(false);
    ui->actionNuevo->setEnabled(false);
    ui->actionBPC->setEnabled(false);
    ui->actionRegresa->setEnabled(false);
    ui->actionSuspendido->setEnabled(false);

    memoria = new QProgressBar*[35]{
                ui->progressBar_1,ui->progressBar_2,ui->progressBar_3,ui->progressBar_4,ui->progressBar_5,
                ui->progressBar_6,ui->progressBar_7,ui->progressBar_8,ui->progressBar_9,ui->progressBar_10,
                ui->progressBar_11,ui->progressBar_12,ui->progressBar_13,ui->progressBar_14,ui->progressBar_15,
                ui->progressBar_16,ui->progressBar_17,ui->progressBar_18,ui->progressBar_19,ui->progressBar_20,
                ui->progressBar_21,ui->progressBar_22,ui->progressBar_23,ui->progressBar_24,ui->progressBar_25,
                ui->progressBar_26,ui->progressBar_27,ui->progressBar_28,ui->progressBar_29,ui->progressBar_30,
                ui->progressBar_31,ui->progressBar_32,ui->progressBar_33,ui->progressBar_34,ui->progressBar_35
            };
    for (int i=0 ; i<35 ; i++){
        memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:white; width: 20px} "
                                  "QProgressBar {border: 2px solid black; color: black; text-align:center; border-radius: 5px}");
        memoria[i]->setMaximum(4);
        memoria[i]->setValue(0);
    }

    //SO
    for (int i=0; i<3 ; i++){
        memo[i].type = 0;
        memo[i].value = 4;
    }
}

void MainWindow::proceed()
{
    node *aux;
    memoryTable();

    if(!blocked.empty()){
        node *aux = blocked.first;
        proceso process;
        bool flag = false;
        while(aux){
            process = aux->process;
            aux = aux->next;
            if(process.blockedTime == 0){
                flag = true;
                blocked.erase(process);
                ui->blockedProcessesTable->removeRow(0);
                blockedProcessesTable();
                process.status = READY;
                ready.push_back(process);

                for (int i=0 ; i<35 ; i++)
                    if (memo[i].ID == process.ID)
                        memo[i].type = READY;
            }
        }
        if(flag && running.empty()){
            nextProcess();
        }
    }

    if(!running.empty()){
        if(running.first->process.getTR() == 0){
            running.first->process.status = FINISHED;
            endProcess();
        }
        if(QD == 0){
            running.first->process.status = READY;
            for (int i=0 ; i<35 ; i++){
                if (memo[i].ID == running.first->process.ID)
                    memo[i].type = READY;
            }
            ready.push_back(running.pop());

            nextProcess();
            readyProcessesTable();
        }
    }

    if (blocked.empty() && ready.empty() && running.empty() && suspendedProcesses.empty()){
        status = FINISHED;
        currentProcessTable();
        updateMemoria();
    }

    if(!running.empty()){
        running.first->process.serviceTime++;
        //QD--;
    }

    if(!ready.empty()){
        aux = ready.first;
        while(aux){
            aux->process.waitTime++;
            aux = aux->next;
        }
    }

    if(!blocked.empty()){
        aux = blocked.first;
        while(aux){
            aux->process.waitTime++;
            aux->process.blockedTime--;
            aux = aux->next;
        }
    }

    if(!suspendedProcesses.empty()){
        aux = suspendedProcesses.first;
        while(aux){
            aux->process.waitTime++;
            aux = aux->next;
        }
    }

    cont++;
    QD--;
}

void MainWindow::memoryTable()
{
    ui->lcdNumber->display(cont);
    ui->lcdNumber_2->display(news.size());
    updateMemoria();
    readyProcessesTable();
    blockedProcessesTable();
    currentProcessTable();
    suspended();
}

void MainWindow::currentProcessTable()
{
    if(!running.empty()){
        ui->IDLabel->setText(QString::number(running.first->process.ID));
        ui->OpLabel->setText(running.first->process.operation);
        ui->TMELabel->setText(QString::number(running.first->process.TME));
        ui->TTLabel->setText(QString::number(running.first->process.serviceTime));
        ui->TRLabel->setText(QString::number(running.first->process.getTR()));
    }
    else{
        ui->IDLabel->setText("-");
        ui->OpLabel->setText("-");
        ui->TMELabel->setText("-");
        ui->TTLabel->setText("-");
        ui->TRLabel->setText("-");
    }
}

void MainWindow::endProcess()
{
    running.first->process.endTime = cont; //endTime
    running.first->process.returnTime = running.first->process.serviceTime + running.first->process.waitTime; //returnTime
    for (int i=0 ; i<35 ; i++){
        if (memo[i].ID == running.first->process.ID){
            memo[i].value = 0;
            memo[i].ID = -1;
        }
    }
    finishedProcesses.push_back(running.pop());
    finishedProcessTable();
    acceptProcess();
    nextProcess();
}

void MainWindow::readyProcessesTable()
{
    int row = 0;
    node *aux = ready.first;
    while (aux){
        ui->readyProcessesTable->removeRow(row);
        ui->readyProcessesTable->insertRow(row);
        ui->readyProcessesTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID))); //ID
        ui->readyProcessesTable->setItem(row,1,new QTableWidgetItem(QString::number(aux->process.TME))); //TMR
        ui->readyProcessesTable->setItem(row,2,new QTableWidgetItem(QString::number(aux->process.getTR())));  //TR

        for (int column=0 ; column<=2 ; column++)
            ui->readyProcessesTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }
}

void MainWindow::blockedProcessesTable()
{
    int row = 0;
    node *aux = blocked.first;
    while (aux){
        ui->blockedProcessesTable->removeRow(row);
        ui->blockedProcessesTable->insertRow(row);
        ui->blockedProcessesTable->setItem(row,0,new QTableWidgetItem(QString::number(aux->process.ID))); //ID
        ui->blockedProcessesTable->setItem(row,1,new QTableWidgetItem(QString::number(aux->process.TME))); //TMR
        ui->blockedProcessesTable->setItem(row,2,new QTableWidgetItem(QString::number(aux->process.getTR())));  //TR
        ui->blockedProcessesTable->setItem(row,3,new QTableWidgetItem(QString::number(aux->process.blockedTime)));  //TBl

        for (int column=0 ; column<=3 ; column++)
            ui->blockedProcessesTable->item(row,column)->setTextAlignment(Qt::AlignCenter);

        row++;
        aux = aux->next;
    }
}

void MainWindow::finishedProcessTable()
{
    int row = ui->finishedProcessesTable->rowCount();
    ui->finishedProcessesTable->insertRow(row);
    ui->finishedProcessesTable->setItem(row,0,new QTableWidgetItem(QString::number(finishedProcesses.last->process.ID)));  //ID
    ui->finishedProcessesTable->setItem(row,1,new QTableWidgetItem(finishedProcesses.last->process.operation));  //OPE
    if (finishedProcesses.last->process.status == FINISHED){
        ui->finishedProcessesTable->setItem(row,2,new QTableWidgetItem(QString::number(finishedProcesses.last->process.result)));  //RESULT
    }
    else if(finishedProcesses.last->process.status == ERROR){
        ui->finishedProcessesTable->setItem(row,2,new QTableWidgetItem("ERROR"));  //RESULT
    }

    for (int column=0 ; column<=2 ; column++)
        ui->finishedProcessesTable->item(row,column)->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::acceptProcess()
{
    if(!news.empty()){
            if (memoryAvailable() >= frames(news.first->process)){
                news.first->process.status = READY;
                news.first->process.arrivalTime = cont;  //arrivalTime
                int f = frames(news.first->process);
                int w = news.first->process.weight;
                int i = 0;
                while (f!=0 && i<35){
                    if (memo[i].value == 0){
                        if(w >= 4){
                            memo[i].value = 4;
                            w -= 4;
                            memo[i].type = READY;
                            memo[i].ID = news.first->process.ID;
                        }
                        else{
                            memo[i].value = w;
                            memo[i].type = READY;
                            memo[i].ID = news.first->process.ID;
                        }
                        f--;
                    }
                    i++;
                }
                qDebug().noquote()<<"Proceso Listo:"<<news.first->process.ID
                                 <<"|TME: "<<news.first->process.TME
                                 <<"|"<<news.first->process.operation
                                 <<"| Peso:"<<news.first->process.weight;
                ready.push_back(news.pop());
            }
        }
}

void MainWindow::nextProcess()
{
    if(!ready.empty()){
        ready.first->process.status = RUNNING;
        if(!ready.first->process.responseTimeFlag){
            ready.first->process.responseTime = ready.first->process.waitTime;
            ready.first->process.responseTimeFlag = true;
        }
        running.push_back(ready.pop());

        for (int i=0 ; i<35 ; i++){
            if (memo[i].ID == running.first->process.ID)
                memo[i].type = RUNNING;
        }
        qDebug().noquote()<<"Proceso en ejecución:"<<running.first->process.ID
                         <<"|TME: "<<running.first->process.TME
                         <<"|"<<running.first->process.operation
                         <<"|Peso:"<<running.first->process.weight;

        ui->readyProcessesTable->removeRow(0);
    }
    QD=quantum;
}

void MainWindow::updateMemoria()
{
    if(!news.empty())
    {
        if(!news.empty())
            ui->label_9->setText("Siguiente proceso:\t ID: "+QString::number(news.first->process.ID)+
                                         "   Peso: "+QString::number(news.first->process.weight));
        else
            ui->label_9->setText("-");
    }
    for (int i=0 ; i<35 ; i++){
        switch (memo[i].type) {
        case 0:     //0 - SO
            memoria[i]->setFormat("SO");
            memoria[i]->setValue(memo[i].value);
            memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:black; width: 20px} "
                                      "QProgressBar {border: 2px solid red; color: white; text-align:center; border-radius: 5px}");
            break;
        case 1:     //1 - READY
            memoria[i]->setFormat("Listo: "+QString::number(memo[i].ID));
            memoria[i]->setValue(memo[i].value);
            memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:blue; width: 20px} "
                                      "QProgressBar {border: 2px solid black; color: black; text-align:center; border-radius: 5px}");
            break;
        case 2:     //2 - RUNNING
            if(memo[i].ID!=-1)
            {
                memoria[i]->setFormat("Ejecución:"+QString::number(memo[i].ID));
                memoria[i]->setValue(memo[i].value);
                memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:red; width: 20px} "
                                          "QProgressBar {border: 2px solid black; color: black; text-align:center; border-radius: 5px}");
            }
            else
            {
                memoria[i]->setFormat("Vacio");
                memoria[i]->setValue(memo[i].value);
                memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:red; width: 20px} "
                                          "QProgressBar {border: 2px solid black; color: black; text-align:center; border-radius: 5px}");
            }

            break;
        case 3:     //3 - BLOCKED
            memoria[i]->setFormat("Bloqueado: "+QString::number(memo[i].ID));
            memoria[i]->setValue(memo[i].value);
            memoria[i]->setStyleSheet("QProgressBar::chunk {background-color:purple; width: 20px} "
                                      "QProgressBar {border: 2px solid black; color: black; text-align:center; border-radius: 5px}");
            break;
        }
    }
}
///////////////////////////////////////////////////////////
void MainWindow::actionNewProcess()
{
    proceso aux;
    aux.new_process(newID);
    news.push_back(aux);
    newID++;

    acceptProcess();
    if(running.empty())
        nextProcess();

    ui->lcdNumber_2->display(news.size());
    updateMemoria();
    readyProcessesTable();
}

void MainWindow::actionInterrupcion()
{
    if(!running.empty()){
        if(running.first->process.getTR() == 0){
            running.first->process.status = FINISHED;
            endProcess();
        }
        if(!running.empty()){
            ui->readyProcessesTable->removeRow(0);
            running.first->process.status = BLOCKED;
            running.first->process.setBlockedTime(BLOCKINGTIME);
            for (int i=0 ; i<35 ; i++){
                if (memo[i].ID == running.first->process.ID)
                    memo[i].type = BLOCKED;
            }

            qDebug().noquote()<<"Proceso bloqueado:"<<running.first->process.ID
                             <<"|TME: "<<running.first->process.TME
                             <<"|"<<running.first->process.operation
                             <<"|Peso:"<<running.first->process.weight;
            blocked.push_back(running.pop());
            nextProcess();
        }
    }
}

void MainWindow::actionError()
{
    if (!running.empty()){
            running.first->process.status = ERROR;
            endProcess();
        }
}

void MainWindow::suspended()
{
    if(suspendedProcesses.empty())
        ui->labelNextSuspended->setText(" ");
    else
        ui->labelNextSuspended->setText("Suspendido por ingresar:\t ID: "+QString::number(suspendedProcesses.first->process.ID)+
                                        "   Peso: "+QString::number(suspendedProcesses.first->process.weight)+
                                        "     Suspendidos: "+QString::number(suspendedProcesses.size()));

    node *aux = suspendedProcesses.first;
    proceso process;
    remove ("suspendidos.txt");
    ofstream file("suspendidos.txt",ios::app);
    while(aux){
        process = aux->process;
        file<<process.ID<<"|"
            <<process.TME<<"|"
            <<process.weight<<"|"
            <<process.operation.toStdString()<<"|"
            <<process.result<<"|"
            <<process.arrivalTime<<"|"
            <<process.responseTime<<"|"
            <<process.waitTime<<"|"
            <<process.serviceTime<<"|"
            <<process.blockedTime<<"|"
            <<process.status<<"|";
        aux = aux->next;
    }
    file.close();
}

bool MainWindow::areThereSuspended()
{
    return true;
}

void MainWindow::actionSuspender()
{
    if(!blocked.empty()){
        proceso process = blocked.pop();
        suspendedProcesses.push_back(process);
        ui->blockedProcessesTable->removeRow(0);

        for (int i=0 ; i<35 ; i++){
            if (memo[i].ID == process.ID){
                memo[i].type=2;
                memo[i].value = 0;
                memo[i].ID = -1;
            }
        }

        qDebug().noquote()<<"\nPROCESO SUSPENDIDO"<<endl
                            <<"ID:"<<process.ID<<endl<<"TME:"
                            <<process.TME<<endl<<"WEIGHT:"
                            <<process.weight<<endl<<"OPERATION:"
                            <<process.operation<<endl<<"RESULT:"
                            <<process.result<<endl<<"ARRIVALTIME:"
                            <<process.arrivalTime<<endl<<"RESPONSETINE:"
                            <<process.responseTime<<endl<<"WAITTIME:"
                            <<process.waitTime<<endl<<"SERVICETIME:"
                            <<process.serviceTime<<endl<<"BLOCKEDTIME:"
                            <<process.blockedTime<<endl<<"STATUS:"
                            <<process.status<<endl;

        ofstream file("suspendidos.txt",ios::app);
        file<<process.ID<<"|"
            <<process.TME<<"|"
            <<process.weight<<"|"
            <<process.operation.toStdString()<<"|"
            <<process.result<<"|"
            <<process.arrivalTime<<"|"
            <<process.responseTime<<"|"
            <<process.waitTime<<"|"
            <<process.serviceTime<<"|"
            <<process.blockedTime<<"|"
            <<process.status<<"|";
        file.close();

        acceptProcess();
        memoryTable();
    }
}

void MainWindow::actionRegresar()
{
    if(!suspendedProcesses.empty()){
        if(memoryAvailable() >= frames(suspendedProcesses.first->process)){
            proceso process = suspendedProcesses.pop();
            int f = frames(process);
            int w = process.weight;
            int i = 0;
            //blocked.push_back(process);
            if(ready.empty())
            {
                running.push_back(process);
                while (f!=0 && i<35){
                    if (memo[i].value == 0){
                        if(w >= 4){
                            memo[i].value = 4;
                            w -= 4;
                            memo[i].type = RUNNING;
                            memo[i].ID = process.ID;
                        }
                        else{
                            memo[i].value = w;
                            memo[i].type = RUNNING; //BL
                            memo[i].ID = process.ID;
                        }
                        f--;
                    }
                    i++;
                }
            }
            else
            {
                ready.push_back(process);
                while (f!=0 && i<35){
                    if (memo[i].value == 0){
                        if(w >= 4){
                            memo[i].value = 4;
                            w -= 4;
                            memo[i].type = READY;
                            memo[i].ID = process.ID;
                        }
                        else{
                            memo[i].value = w;
                            memo[i].type = READY; //BL
                            memo[i].ID = process.ID;
                        }
                        f--;
                    }
                    i++;
                }
            }




            memoryTable();

            qDebug().noquote()<<"\nPROCESO DE REGRESO"<<endl
                                <<"ID:"<<process.ID<<endl<<"TME:"
                                <<process.TME<<endl<<"WEIGHT:"
                                <<process.weight<<endl<<"OPERATION:"
                                <<process.operation<<endl<<"RESULT:"
                                <<process.result<<endl<<"ARRIVALTIME:"
                                <<process.arrivalTime<<endl<<"RESPONSETIME:"
                                <<process.responseTime<<endl<<"WAITTIME:"
                                <<process.waitTime<<endl<<"SERVICETIME:"
                                <<process.serviceTime<<endl<<"BLOCKEDTIME:"
                                <<process.blockedTime<<endl<<"STATUS:"
                                <<process.status<<endl;

            node *aux = suspendedProcesses.first;
            remove ("suspendidos.txt");
            ofstream file("suspendidos.txt",ios::app);
            while(aux){
                process = aux->process;
                file<<process.ID<<"|"
                    <<process.TME<<"|"
                    <<process.weight<<"|"
                    <<process.operation.toStdString()<<"|"
                    <<process.result<<"|"
                    <<process.arrivalTime<<"|"
                    <<process.responseTime<<"|"
                    <<process.waitTime<<"|"
                    <<process.serviceTime<<"|"
                    <<process.blockedTime<<"|"
                    <<process.status<<"|";
                aux = aux->next;
            }
            file.close();
        }
        else{
            qDebug().noquote()<<endl<<"No hay espacio."<<endl;
        }
    }
    else{
        qDebug().noquote()<<endl<<"No hay procesos suspendidos."<<endl;
    }

}
///////////////////////////////////////////////////////////
void MainWindow::on_pushButtonStart_clicked()
{
    ui->pushButtonStart->setEnabled(false);

    running.push_back(news.pop());
    running.first->process.arrivalTime = cont; //arrivalTime
    running.first->process.status = RUNNING;
    running.first->process.responseTime = cont;
    running.first->process.responseTimeFlag = true;

    if (memoryAvailable() >= frames(running.first->process)){
        int f = frames(running.first->process);
        int w = running.first->process.weight;
        int i = 0;
        while (f!=0 && i<35){
            if (memo[i].value == 0){
                if(w >= 4){
                    memo[i].value = 4;
                    w -= 4;
                    memo[i].type = RUNNING;
                    memo[i].ID = running.first->process.ID;
                }
                else{
                    memo[i].value = w;
                    memo[i].type = RUNNING;
                    memo[i].ID = running.first->process.ID;
                }
                f--;
            }
            i++;
        }
    }

    ////
    node *aux = news.first;
    proceso auxProcess;
    bool flag;
    while(aux){
        flag = false;
        if(memoryAvailable() >= frames(aux->process)){
            auxProcess = aux->process;
            int f = frames(aux->process);
            int w = aux->process.weight;
            int i = 0;
            while(f!=0 && i<35){
                if(memo[i].value == 0){
                    if(w >=4){
                        memo[i].value = 4;
                        w -= 4;
                        memo[i].type = READY;
                        memo[i].ID = aux->process.ID;
                    }
                    else{
                        memo[i].value = w;
                        memo[i].type = READY;
                        memo[i].ID = aux->process.ID;
                    }
                    f--;
                }
                i++;
            }
            flag = true;
        }

        aux=aux->next;
        if (flag){
            auxProcess.status = READY;
            auxProcess.arrivalTime = cont;
            ready.push_back(news.popPos(auxProcess));
        }
    }
    status = RUNNING;

    ui->actionError->setEnabled(true);
    ui->actionInterrupci_n->setEnabled(true);
    ui->actionPausa->setEnabled(true);
    ui->actionNuevo->setEnabled(true);
    ui->actionBPC->setEnabled(true);
    ui->actionSuspendido->setEnabled(true);
    ui->actionRegresa->setEnabled(true);
}

void MainWindow::on_pushButtonAdd_clicked()
{
    int res,nALote;
    QMessageBox Msgbox;
    QString ID2,signo,Nombre;
    Datafill Filler(this);
    Filler.setWindowTitle("Llenado de informacion");
    res = Filler.exec();
    if(res == QDialog::Rejected)
           return;
    nALote = Filler.Operacion();
    if(Filler.Q!=0 && nALote!=0)
    {
        quantum = Filler.Q;
        QD=quantum;
        ui->lcdNumber_3->display(quantum);
        //ui->lcdNumber->display(nALote);
        //
        proceso aux;
        for (int i = 1 ; i <= nALote ; i++){
            aux.new_process(newID);
            news.push_back(aux);
            newID++;
        }
        //Procesos aux;
        ui->actionNuevo->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonAdd->setEnabled(false);
        //ui->pushButton_2->setEnabled(true);
        //ui->pushButton->setEnabled(false);
    }
    else
    {
        Msgbox.setText("El Quantum o el número de procesos fue ingresado en 0, ingrese un numero positivo en ambos.");
        Msgbox.exec();
        on_pushButtonAdd_clicked();
    }
    /*
    ui->pushButtonAdd->setEnabled(false);
    ui->spinBoxNProcess->setEnabled(false);
    ui->spinBoxQuantum->setEnabled(false);

    quantum = ui->spinBoxQuantum->value();

    proceso aux;
    for (int i = 1 ; i <= ui->spinBoxNProcess->value() ; i++){
        aux.new_process(newID);
        news.push_back(aux);
        newID++;
    }

    //ui->statusBar->showMessage("PROCESOS AGREGADOS");
    ui->pushButtonStart->setEnabled(true);
    */
}

void MainWindow::update()
{

    switch (status) {
        case RUNNING:
            switch(action){
            case 1:                 //Nuevo
                actionNewProcess();
                break;
            case 2:
                actionInterrupcion();   //Interrupción
                break;
            case 3:
                actionError();      //error
                break;
            case 4:                 //suspender
                actionSuspender();
                break;
            case 5:                 //regresar
                actionRegresar();
                break;
            }
            action = 99999;
            proceed();
            break;
        case FINISHED:

            ui->actionContinuar->setEnabled(false);
            ui->actionError->setEnabled(false);
            ui->actionInterrupci_n->setEnabled(false);
            ui->actionPausa->setEnabled(false);
            ui->actionBPC->setEnabled(false);
            ui->actionNuevo->setEnabled(false);
            ui->actionSuspendido->setEnabled(false);
            ui->actionRegresa->setEnabled(false);
            break;
        case PAUSED:
            break;
        }
    /*
    switch (status) {
    case RUNNING:
        proceed();
        //ui->statusBar->showMessage("PROCESANDO");
        break;
    case FINISHED:
        //ui->statusBar->showMessage("FINALIZADO");

        ui->actionContinuar->setEnabled(false);
        ui->actionError->setEnabled(false);
        ui->actionInterrupci_n->setEnabled(false);
        ui->actionPausa->setEnabled(false);
        ui->actionBPC->setEnabled(false);
        break;
    case PAUSED:
        //ui->statusBar->showMessage("PAUSADO");
        break;
    }
    */
}

void MainWindow::on_pushButtonFinalTable_clicked()
{
    finalTableWindow->finishedProcessesTable(finishedProcesses);
    finalTableWindow->show();
}

void MainWindow::on_actionPausa_triggered()
{
    status = PAUSED;
    ui->actionContinuar->setEnabled(true);
    ui->actionError->setEnabled(false);
    ui->actionInterrupci_n->setEnabled(false);
    ui->actionPausa->setEnabled(false);

    for(int i=0 ; i<35 ; i++){
        qDebug().noquote()<<i+1<<"-"<<memo[i].ID;
    }
}

void MainWindow::on_actionContinuar_triggered()
{
    status = RUNNING;
    ui->actionContinuar->setEnabled(false);
    ui->actionError->setEnabled(true);
    ui->actionInterrupci_n->setEnabled(true);
    ui->actionPausa->setEnabled(true);
    if (currentBCP->isActiveWindow()){
        currentBCP->close();
    }
}

void MainWindow::on_actionError_triggered()
{
    action = 3;
}

void MainWindow::on_actionInterrupci_n_triggered()
{
    action = 2;
}

void MainWindow::on_actionNuevo_triggered()
{
    actionNewProcess();
}

void MainWindow::on_actionBPC_triggered()
{
    on_actionPausa_triggered();
    if (!currentBCP->isActiveWindow()){
        currentBCP->loadMemoryTable(running,ready,blocked, news, finishedProcesses,suspendedProcesses);
        currentBCP->show();
    }
    currentProcessTable();
    blockedProcessesTable();
    ui->lcdNumber->display(cont);
    //ui->labelCont->setText("Cont = " + QString::number(cont));
}

int MainWindow::memoryAvailable()
{
    available = 0;
    for(int i=1 ; i<35 ; i++)
        if (memo[i].value == 0)
            available++;
    return available;
}

int MainWindow::frames(proceso process)
{
    int frames = process.weight / 4;

    if (process.weight % 4 != 0)
        frames++;

    return  frames;
}

void MainWindow::on_actionTablaP_triggered()
{
    status = PAUSED;
    ui->actionContinuar->setEnabled(true);
    ui->actionError->setEnabled(false);
    ui->actionInterrupci_n->setEnabled(false);
    ui->actionPausa->setEnabled(false);

    for(int i=0 ; i<35 ; i++){
        qDebug().noquote()<<i+1<<"-"<<memo[i].ID;
    }
}

void MainWindow::on_actionSuspendido_triggered()
{
    action = 4;
}

void MainWindow::on_actionRegresa_triggered()
{
    action = 5;
}
