#include "headhomewarningdialog.h"

HeadHomeWarningDialog::HeadHomeWarningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadHomeWarningDialog)
{
    ui->setupUi(this);
}

HeadHomeWarningDialog::~HeadHomeWarningDialog()
{
    delete ui;
}
