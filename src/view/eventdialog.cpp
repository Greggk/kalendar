#include "eventdialog.h"

#include <QDebug>

void EventDialog::setEvent(Event *event) {
    this->everyMonth->setEnabled(false); /* Not supported for now */
    this->everyYear->setEnabled(false);
    this->everyMonthDay->setEnabled(false);
    this->everyYearDay->setEnabled(false);
    this->everyWeek->setEnabled(false);
    this->event = event;
    if (event != NULL) {
        this->edit_name->setText(event->getName().c_str());
        this->edit_description->setPlainText(event->getDescription().c_str());
        this->edit_place->setText(event->getPlace().c_str());
        int index = 0;
        for (Category *category : category_list) {
            if (category->getName() == event->getCategory()->getName())
                this->edit_category->setCurrentIndex(index);
            index++;
        }

        if (event->getStart() == TODO_DATE) {
            Date today = DateUtil::get_current_date();
            /* Set the current date, so if the user uncheck the "TODO" there is already a valid date set */
            this->edit_start->setDateTime(QDateTime(QDate(today.getYear(), today.getMonth(), today.getMonthDay())));
            this->edit_start->setEnabled(false);
            this->edit_end->setDateTime(QDateTime(QDate(today.getYear(), today.getMonth(), today.getMonthDay())));
            this->edit_end->setEnabled(false);
            this->cbtodo->setChecked(true);
        } else {
            this->edit_start->setDateTime(QDateTime::fromTime_t(event->getStart()));
            this->edit_end->setDateTime(QDateTime::fromTime_t(event->getEnd()));
        }
        this->button_delete->setEnabled(true);
    }
}

EventDialog::EventDialog(View *parentView, Date start_date, Date end_date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    this->parent = parentView;
    this->setFixedWidth(400);
    this->setFixedHeight(500);
    this->setModal(true);
    this->setWindowTitle(tr("Event Manager"));

    this->pm = new SecurePManager;
    this->event = NULL;

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *first_row = new QHBoxLayout;
    QLabel *label_name = new QLabel(tr("Name: "));
    this->edit_name = new QLineEdit;
    this->edit_name->setMaxLength(100);
    first_row->addWidget(label_name);
    first_row->addWidget(this->edit_name);
    main_layout->addLayout(first_row);
    QHBoxLayout *second_row = new QHBoxLayout;
    QLabel *label_place = new QLabel(tr("Place: "));
    this->edit_place = new QLineEdit;
    second_row->addWidget(label_place);
    second_row->addWidget(this->edit_place);
    main_layout->addLayout(second_row);
    main_layout->addWidget(new QLabel(tr("Description: ")));
    this->edit_description = new QPlainTextEdit;
    main_layout->addWidget(this->edit_description);
    QHBoxLayout *third_row = new QHBoxLayout;
    QLabel *label_category = new QLabel(tr("Category: "));
    this->edit_category = new QComboBox;
    this->category_list = this->pm->get_categories();
    for (Category *category : category_list) {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->edit_category->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
    third_row->addWidget(label_category);
    third_row->addWidget(this->edit_category);
    main_layout->addLayout(third_row);
    QHBoxLayout *fourth_row = new QHBoxLayout;
    QLabel *label_todo = new QLabel(tr("TODO: "));
    this->cbtodo = new QCheckBox;
    connect(cbtodo, &QCheckBox::toggled, this, &EventDialog::on_checkbox_todo_toggle);
    fourth_row->addWidget(label_todo);
    fourth_row->addWidget(cbtodo);
    main_layout->addLayout(fourth_row);
    QHBoxLayout *fifth_row = new QHBoxLayout;
    QLabel *label_start = new QLabel(tr("Start: "));
    this->edit_start = new QDateTimeEdit;
    this->edit_start->setCalendarPopup(true);
    QDateTime todoDate = QDateTime::fromTime_t(TODO_DATE);
    this->edit_start->setDateTime(QDateTime(QDate(start_date.getYear(), start_date.getMonth(), start_date.getMonthDay())));
    //I set a specific hour because the default is the midnight, but this could lead more easily problems caused by daylight saving time
    this->edit_start->setTime(QTime(8,0,0));
    connect(this->edit_start, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::on_date_start_change);
    fifth_row->addWidget(label_start);
    fifth_row->addWidget(this->edit_start);
    main_layout->addLayout(fifth_row);
    QHBoxLayout *sixth_row = new QHBoxLayout;
    QLabel *label_end = new QLabel(tr("End: "));
    this->edit_end = new QDateTimeEdit;
    this->edit_end->setCalendarPopup(true);
    this->edit_end->setDateTime(QDateTime(QDate(end_date.getYear(), end_date.getMonth(), end_date.getMonthDay())));
    this->edit_end->setTime(QTime(9,0,0));
    sixth_row->addWidget(label_end);
    sixth_row->addWidget(this->edit_end);
    main_layout->addLayout(sixth_row);
    QVBoxLayout *seventh_row = new QVBoxLayout;
    QLabel *label_recurrent = new QLabel(tr("Recurrence: "));
    this->everyMonth = new QRadioButton(tr("Monthly-Date"));
    this->everyYear = new QRadioButton(tr("Yearly-Date"));
    this->everyMonthDay = new QRadioButton(tr("Monthly-WeekDay"));
    this->everyYearDay = new QRadioButton(tr("Yearly-WeekDay"));
    this->everyWeek = new QRadioButton(tr("Weekly"));
    this->options = new QGroupBox;
    seventh_row->addWidget(label_recurrent);
    seventh_row->addWidget(this->everyMonth);
    seventh_row->addWidget(this->everyYear);
    seventh_row->addWidget(this->everyMonthDay);
    seventh_row->addWidget(this->everyYearDay);
    seventh_row->addWidget(this->everyWeek);
    options->setLayout(seventh_row);
    main_layout->addWidget(options);
    QHBoxLayout *last_row = new QHBoxLayout;
    QPushButton *button_cancel = new QPushButton(tr("&Cancel"));
    connect(button_cancel, &QPushButton::clicked, this, &EventDialog::on_button_cancel_click);
    button_delete = new QPushButton(tr("&Delete"));
    button_delete->setEnabled(false);
    connect(button_delete, &QPushButton::clicked, this, &EventDialog::on_button_delete_click);
    QPushButton *button_save = new QPushButton(tr("&Save"));
    connect(button_save, &QPushButton::clicked, this, &EventDialog::on_button_save_click);
    last_row->addWidget(button_cancel);
    last_row->addWidget(button_delete);
    last_row->addWidget(button_save);
    main_layout->addLayout(last_row);

    this->setLayout(main_layout);
}

EventDialog::~EventDialog()
{
    delete ui;
    delete this->pm;
    for (Category *c : this->category_list) delete c;
}

void EventDialog::on_button_cancel_click() {
    this->close();
    delete this;
}

void EventDialog::on_button_delete_click() {
    this->pm->remove_event(this->event);
    this->event->setInvalid();
    refresh();
}

void EventDialog::refresh() {
    this->parent->refresh_events();
    this->close();
    delete this;
}

void EventDialog::on_button_save_click() {
    QProgressBar *bar = new QProgressBar;
    bool ret = true;
    bool isTodo = !this->edit_start->isEnabled();

    if (this->edit_name->text().length() < 3) {
        QMessageBox::critical(this, tr("Error"), tr("The name must have a length greater than 2"), QMessageBox::Ok);
        return;
    }

    if (isTodo) {
        QDateTime todoDate = QDateTime::fromTime_t(TODO_DATE);
        this->edit_start->setDateTime(todoDate);
        this->edit_end->setDateTime(todoDate);
    } else {
        if (this->edit_start->dateTime() > this->edit_end->dateTime()) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid range of time selected"), QMessageBox::Ok);
            return;
        }
    }

    Category *category = NULL;
    for (Category *c : this->category_list) {
        if (this->edit_category->currentText().toStdString() == c->getName()) {
            category = new Category(*c);
            break;
        }
    }

    QDateTime start = this->edit_start->dateTime();
    QDateTime end = this->edit_end->dateTime();
    Event *newEvent = new Event(0, this->edit_name->text().toStdString(), this->edit_description->toPlainText().toStdString(), this->edit_place->text().toStdString(), category, start.toTime_t(), end.toTime_t());

    /* If the users has changed an existent event, I'll call the right function */
    if ((this->event != NULL) && (this->pm->replace_event(this->event, newEvent))) {
        refresh();
        this->event = new Event(*newEvent);
    } else if ((this->event == NULL) && (this->everyMonth->isChecked() || this->everyYear->isChecked() ||
                                         this->everyMonthDay->isChecked() || this->everyYearDay->isChecked() ||
                                         this->everyWeek->isChecked())) {
        int reply = QMessageBox::warning(this, tr("Attention"), tr("A recurrent event is considered as multiple independent events, after this operation you can modify it only as a single event. Recurrent events by Weekday not supported in week 5"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            int month_week = 0;
            int month_week_next = 0;
            if (this->everyMonthDay->isChecked() || this->everyYearDay->isChecked()) {
                if (start.date().day() <= 7)
                    month_week = 1;
                else if (start.date().day() > 7 && start.date().day() <= 14)
                     month_week = 2;
                else if (start.date().day() > 14 && start.date().day() <= 21)
                     month_week = 3;
                else if (start.date().day() > 21 && start.date().day() <= 28)
                    month_week = 4;
                else
                    month_week = 5;
            }
            int offset = 8; /* Add the same event to the next 8 years */
            if (this->everyMonth->isChecked() || this->everyMonthDay->isChecked())
                offset = 24; /* Add the same event to every month of the next 2 years */
            if (this->everyWeek->isChecked())
                offset = 52; /* Add the same event to every week of the next year */
            bar->setRange(0, offset);
            this->layout()->addWidget(bar);
            Event *previous = NULL;
            for (int i = 0; i < offset; i++) {
                ret = ret && this->pm->add_event(newEvent, previous);
                if (previous != NULL) delete previous;
                previous = newEvent;
                if (!ret) break;
                if (this->everyMonth->isChecked()) {
                    start = start.addMonths(1);
                    end = end.addMonths(1);
                } else if (this->everyMonthDay->isChecked() && month_week != 5) {
                    start = start.addDays(28);
                    end = end.addDays(28);
                    if (start.date().day() <= 7)
                        month_week_next = 1;
                    else if (start.date().day() > 7 && start.date().day() <= 14)
                         month_week_next = 2;
                    else if (start.date().day() > 14 && start.date().day() <= 21)
                         month_week_next = 3;
                    else if (start.date().day() > 21 && start.date().day() <= 28)
                        month_week_next = 4;
                    else
                        month_week_next = 5;
                    if (month_week != month_week_next) {
                        start = start.addDays(7);
                        end = end.addDays(7);
                    }
                } else if (this->everyYearDay->isChecked() && month_week != 5) {
                    start = start.addDays(364);
                    end = end.addDays(364);
                    if (start.date().day() <= 7)
                        month_week_next = 1;
                    else if (start.date().day() > 7 && start.date().day() <= 14)
                        month_week_next = 2;
                    else if (start.date().day() > 14 && start.date().day() <= 21)
                        month_week_next = 3;
                    else if (start.date().day() > 21 && start.date().day() <= 28)
                        month_week_next = 4;
                    else
                        month_week_next = 5;
                    if (month_week != month_week_next) {
                        start = start.addDays(7);
                        end = end.addDays(7);
                    }
                } else if (this->everyWeek->isChecked()) {
                    start = start.addDays(7);
                    end = end.addDays(7);
                } else {
                    start = start.addYears(1);
                    end = end.addYears(1);
                }
                newEvent = new Event(0, this->edit_name->text().toStdString(), this->edit_description->toPlainText().toStdString(), this->edit_place->text().toStdString(), category, start.toTime_t(), end.toTime_t());
                bar->setValue(i);
                QCoreApplication::processEvents();
            }
            delete previous;
            if (ret) {
                delete newEvent;
                refresh();
            }
        }
    } else if ((this->event == NULL) && (this->pm->add_event(newEvent))) { //else I'll create a new Event
        delete newEvent;
        refresh();
    } else
        ret = false;

    if (!ret) {
        QMessageBox::critical(this, tr("Error"), tr("Persistence error. Try with a different name."), QMessageBox::Ok);
        delete bar;
    }
}

void EventDialog::on_checkbox_todo_toggle(bool checked) {
    this->edit_start->setEnabled(!checked);
    this->edit_end->setEnabled(!checked);
    this->options->setEnabled(!checked);
}

void EventDialog::on_date_start_change() {
    if (this->edit_start->date() > this->edit_end->date())
        this->edit_end->setDate(this->edit_start->date());
    if (this->edit_start->time() > this->edit_end->time())
        this->edit_end->setTime(this->edit_start->time().addSecs(60*60));
}

Event* EventDialog::getEvent() {
    return this->event;
}
