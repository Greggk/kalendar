#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include "ui_eventdialog.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QComboBox>
#include <QMessageBox>
#include <QIconEngine>
#include <QPixmap>
#include <QIcon>
#include "util/dateutil.h"
#include "view.h"
#include "persistence/securepmanager.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::EventDialog *ui;
    QLineEdit *edit_name;
    QLineEdit *edit_place;
    QComboBox *edit_category;
    QDateTimeEdit *edit_start;
    QDateTimeEdit *edit_end;
    QPlainTextEdit *edit_description;
    QPushButton *button_delete;
    QCheckBox *cbtodo;
    QRadioButton *everyMonth;
    QRadioButton *everyYear;
    QRadioButton *everyMonthDay;
    QRadioButton *everyYearDay;
    QRadioButton *everyWeek;
    QGroupBox *options;
    View *parent;
    PManager *pm;
    Event *event;
    vector<Category *> category_list;
    void refresh();

public:
    explicit EventDialog(View *parentView, Date start_date = Date(), Date end_date = Date(), QWidget *parent = 0);
    ~EventDialog();
    void setEvent(Event *event);
    Event* getEvent();

public slots:
    void on_button_cancel_click();
    void on_button_save_click();
    void on_button_delete_click();
    void on_date_start_change();
    void on_checkbox_todo_toggle(bool checked);
};

#endif // EVENTDIALOG_H
