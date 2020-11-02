#ifndef CURRENTBCPTABLE_H
#define CURRENTBCPTABLE_H

#include <QDialog>
#include <processeslist.h>

namespace Ui {
class CurrentBCPTable;
}

class CurrentBCPTable : public QDialog
{
    Q_OBJECT

public:
    explicit CurrentBCPTable(QWidget *parent = nullptr);
    ~CurrentBCPTable();

    void loadMemoryTable(processesList running, processesList ready, processesList blocked,processesList news ,processesList finished,processesList x);
    void prepareTable();

private:
    Ui::CurrentBCPTable *ui;
};

#endif // CURRENTBCPTABLE_H
