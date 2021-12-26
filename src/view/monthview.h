#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QListIterator>
#include <QColor>
#include <QProgressBar>
#include <QtConcurrent/QtConcurrent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDir>
#include <QFileDialog>
#include <QScrollArea>
#include <QInputDialog>
#include <QSettings>
#include "ui_monthview.h"
#include "view/eventdialog.h"
#include "view/qframe_extended.h"
#include "view/qwidget_extended.h"
#include "view/categorydialog.h"
#include "view.h"
#include "util/dateutil.h"
#include "qlabel_event.h"
#include "qpushbutton_extended.h"
#include "view/customdialog.h"
#include "view/categoryselectdialog.h"
#include "util/pluginmanager.h"
#include "view/settings.h"

/* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted.
 * This should be used when you don't care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

/* Style Sheets */
#define MAINWINDOW_STYLE "#mainwindow { background-color: #FFFFFF; border: 2px solid #DDDDFF; }"

#define LABELDATE_STYLE "QLabel { color: #000000; qproperty-alignment: AlignCenter; padding-left: 100px; padding-right: 100px; font-size: 20px; }"

#define TODOSBUTTON_STYLE "QPushButton { border: 1px solid #000000; color: #000000; padding: 5px; font-weight: bold; border-radius: 10px; } QPushButton:hover { color: #2222BB; }"

#define CELL_STYLE "QFrame { background-color: #FFFFFF; color: #000000; border: 1px outset #CCCCCC; }" \
                   "QFrame#holiday { background-color: #FFF5F5; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; color: #FF0000; border: 1px solid #FF0000;}" \
                   "QFrame#selected { background-color: #EEEEFF; }" \
                   "QLabel#header { font-weight: bold; }" \
                   "QFrame#header { background-color: #DDDDFF; }" \
                   "QFrame#disabled { background-color: #EFEFEF; }"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow, public View
{
    Q_OBJECT

private:
    QSettings settings;
    Category *selected_category;
    PluginManager *plm;
    QVBoxLayout *layout;
    QLabel *label_date;
    QLabelEvent *selected_event;
    QPushButton *todobutton;
    Date *selection_start;
    Date *selection_end;
    Ui::MonthView *ui;
    PManager *pm;
    QFrameExtended *frames[42]; //I have a 7x7 grid, but without consider the first row I've a total of 6x7 cells
    QMenu *dbMenu;
    void display_days(Date date);
    void remove_events_from_all_frames();
    void remove_events_from_frame(int i);
    QLabelEvent *createLabelEvent(Event *event);
    QFrameExtended *createQFrameExtended(Date *date);
    void createMenu();
    void createStatusBar();
    void on_button_extended_click(int index);
    CustomDialog *show_progress_bar(QString title);
    void refresh_db_menu();

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

public:
    explicit MonthView(QWidget *parent = 0);
    void refresh_events();
    void refresh_todos();
    void display_events(Date date, Category *category = NULL);
    ~MonthView();

public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);
    void on_mouse_move(QFrameExtended *frame);
    void on_back_button_click();
    void on_next_button_click();
    void on_event_click(QLabelEvent *label_event, Qt::MouseButton button);
    void on_event_key_press(int key);
    void on_todo_button_click();
    void keyPressEvent(QKeyEvent* e);


private slots:
    void exit();
    void delete_db();
    void save_database();
    void load_database();
    void import_events();
    void export_events();
    void add_event();
    void edit_categories();
    void show_agenda(bool only_todos = false);
    void show_monthview();
    void filter_by_category();
    void run_tool(string tool);
    void switch_db(string db);
    void create_database();
    void show_settings();
};

#endif // MONTHVIEW_H
