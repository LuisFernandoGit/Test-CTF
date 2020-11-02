#ifndef FINALTABLE_H
#define FINALTABLE_H

#include <QDialog>
#include <processeslist.h>

namespace Ui {
class Finaltable;
}

class Finaltable : public QDialog
{
    Q_OBJECT

public:
    explicit Finaltable(QWidget *parent = nullptr);
    ~Finaltable();
    void finishedProcessesTable(processesList finishedProcesses);

private:
    Ui::Finaltable *ui;

    void prepareTable();

};

#endif // FINALTABLE_H
