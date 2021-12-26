#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"
#include <QDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLayout>
#include <QSettings>
#include "../persistence/securepmanager.h"

namespace Ui {
class Settings;
}

struct SettingsValues {
    /* initialization in settings.cpp */
    static const QString past_months_expiration;
};

class Settings : public QDialog
{
    Q_OBJECT

private:
    QSpinBox *pastMonthsExpiration;
    Ui::Settings *ui;
    PManager *pm;

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void on_button_cancel_click();
    void on_button_save_click();

protected:

};

#endif // SETTINGS_H
