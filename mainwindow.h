#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "proceso.h"
#include "processeslist.h"
#include "finaltable.h"
#include "currentbcptable.h"
#include "memory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButtonAdd_clicked();

    void update();

    void on_pushButtonFinalTable_clicked();

    void on_actionPausa_triggered();

    void on_actionContinuar_triggered();

    void on_actionError_triggered();

    void on_actionInterrupci_n_triggered();

    void on_actionNuevo_triggered();

    void on_actionBPC_triggered();

    void on_actionTablaP_triggered();

    void on_actionSuspendido_triggered();

    void on_actionRegresa_triggered();

private:
    Ui::MainWindow *ui;

    int cont,status,newID,quantum,available,QD,action;
    QProgressBar** memoria;
    memory memo[35];
    processesList news;                 //vector<proceso> news;
    processesList blocked;              //vector<proceso> blocked;
    processesList ready;                //vector<proceso> ready;
    processesList running;              //vector<proceso> running;
    processesList finishedProcesses;    //vector<proceso> finishedProcesses;
    processesList suspendedProcesses;
    QTimer *timer;
    finaltable *finalTableWindow;
    currentBCPTable *currentBCP;

    int memoryAvailable();
    int frames(proceso process);

    void prepare_inputs();
    void proceed();
    void memoryTable();
    void currentProcessTable();
    void endProcess();
    void readyProcessesTable();
    void blockedProcessesTable();
    void finishedProcessTable();
    void searchProcessReady();
    void acceptProcess();
    void nextProcess();
    void updateMemoria();
    void actionNewProcess();
    void actionInterrupcion();
    void actionError();
    void suspended();
    bool areThereSuspended();
    void actionSuspender();
    void actionRegresar();
};

#endif // MAINWINDOW_H
