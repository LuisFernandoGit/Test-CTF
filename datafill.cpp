#include <datafill.h>
#include <ui_datafill.h>

Datafill::Datafill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Datafill)
{
    ui->setupUi(this);
}

Datafill::~Datafill()
{
    delete ui;
}

void Datafill::on_buttonBox_accepted()
{
    accept();
}

void Datafill::on_buttonBox_rejected()
{
    reject();
}
int Datafill::Operacion() const
{
    return ui->spinBox->value();
}

void Datafill::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    Q=value;
}
