#ifndef CURRENTBCPTABLE_H
#define CURRENTBCPTABLE_H

#include <QDialog>
#include "processeslist.h"

namespace Ui {
class currentBCPTable;
}

class currentBCPTable : public QDialog
{
    Q_OBJECT

public:
    explicit currentBCPTable(QWidget *parent = nullptr);
    ~currentBCPTable();

    void loadMemoryTable(processesList running, processesList ready, processesList blocked,processesList news ,processesList finished,processesList x);
    void prepareTable();

private:
    Ui::currentBCPTable *ui;
};

#endif // CURRENTBCPTABLE_H
