#ifndef DATAFILL_H
#define DATAFILL_H

#include <QDialog>

namespace Ui {
class Datafill;
}

class Datafill : public QDialog
{
    Q_OBJECT

public:
    explicit Datafill(QWidget *parent = nullptr);
    ~Datafill();
    int Operacion() const;
    int Q;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Datafill *ui;
};

#endif // DATAFILL_H
