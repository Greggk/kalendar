#include "settings.h"
#include <stdlib.h>
#include <QDebug>

const QString SettingsValues::past_months_expiration = "PastMonthsExpiration";

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    //SettingsValues::past_months_expiration.setNum(10);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    this->setWindowTitle("Settings");
    this->pm = new PManager;
    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *row1 = new QHBoxLayout;
    QLabel *lblPastMonthExpiration = new QLabel("Delete all events older than (months): ");
    this->pastMonthsExpiration = new QSpinBox;
    pastMonthsExpiration->setRange(0, 120);
    pastMonthsExpiration->setSingleStep(1);
    pastMonthsExpiration->setValue(this->pm->get_setting(1));
    pastMonthsExpiration->setFixedWidth(100);
    row1->addWidget(lblPastMonthExpiration);
    row1->addWidget(pastMonthsExpiration);
    QHBoxLayout *lastRow = new QHBoxLayout;
    QPushButton *btnCancel = new QPushButton("Cancel");
    QPushButton *btnSave = new QPushButton("Save");
    connect(btnCancel, &QPushButton::clicked, this, &Settings::on_button_cancel_click);
    connect(btnSave, &QPushButton::clicked, this, &Settings::on_button_save_click);
    lastRow->addWidget(btnCancel);
    lastRow->addWidget(btnSave);
    main_layout->addLayout(row1);
    main_layout->addLayout(lastRow);
    setLayout(main_layout);
}

Settings::~Settings()
{
    delete ui;
    delete pm;
}

void Settings::on_button_cancel_click() {
    //printf("in cancel");
    this->close();
}

void Settings::on_button_save_click() {
    QSettings settings;
    //settings.setValue(
    //SettingsValues::past_months_expiration.clear();
    this->pm->replace_setting(1, this->pastMonthsExpiration->value());
    //printf(" get %u", pm->get_setting(1));
    //printf("in save value %u", this->pastMonthsExpiration->value());
    this->close();
}


