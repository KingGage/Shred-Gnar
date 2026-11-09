/******************************************************************************
 * Project File:    HeadHomeDialog.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The dialogue that shows up notifying the user progress will be lost
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef HEADHOMEDIALOG_H
#define HEADHOMEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "ui_headhomedialog.h"

namespace Ui { class HeadHomeDialog; }

class HeadHomeDialog : public QDialog{
    Q_OBJECT

public:
    explicit HeadHomeDialog(QWidget *parent = nullptr);
    ~HeadHomeDialog();

private:
    Ui::HeadHomeDialog *ui;
};

#endif // HEADHOMEDIALOG_H
