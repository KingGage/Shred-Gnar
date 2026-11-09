#ifndef HEADHOMEWARNINGDIALOG_H
#define HEADHOMEWARNINGDIALOG_H

#include <QDialog>

namespace Ui {
class HeadHomeWarningDialog;
}

class HeadHomeWarningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadHomeWarningDialog(QWidget *parent = nullptr);
    ~HeadHomeWarningDialog();

private:
    Ui::HeadHomeWarningDialog *ui;
};

#endif // HEADHOMEWARNINGDIALOG_H
