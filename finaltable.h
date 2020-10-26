#ifndef FINALTABLE_H
#define FINALTABLE_H

#include <QDialog>
#include "processeslist.h"

namespace Ui {
class finaltable;
}

class finaltable : public QDialog
{
    Q_OBJECT

public:
    explicit finaltable(QWidget *parent = nullptr);
    ~finaltable();
    void finishedProcessesTable(processesList finishedProcesses);

private:
    Ui::finaltable *ui;

    void prepareTable();

};

#endif // FINALTABLE_H
