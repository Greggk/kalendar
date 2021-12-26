#include "categorydialog.h"

#include <QDebug>
#include <functional>

CategoryDialog::CategoryDialog(View *parentView, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)
{
    this->parent = parentView;
    this->setFixedWidth(300);
    this->setFixedHeight(400);
    this->setWindowTitle(tr("Category Manager"));
    this->pm = new PManager;
    QVBoxLayout *layout = new QVBoxLayout;
    this->list_widget = new QListWidget;
    connect(this->list_widget, &QListWidget::itemClicked, this, &CategoryDialog::on_item_click);
    load_categories();
    QPushButton *button_add = new QPushButton(tr("&Add new category"));
    QPushButton *button_cancel = new QPushButton(tr("&Cancel"));
    connect(button_cancel, &QPushButton::clicked, this, &CategoryDialog::on_button_cancel_click);
    connect(button_add, &QPushButton::clicked, this, &CategoryDialog::on_button_add_click);
    layout->addWidget(this->list_widget);
    layout->addWidget(button_add);
    layout->addWidget(button_cancel);
    this->setLayout(layout);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
    delete pm;
    for (Category *category : this->category_list) delete category;
}

void CategoryDialog::load_categories() {
    for (Category *category : this->category_list) delete category;
    this->category_list = this->pm->get_categories();
    this->list_widget->clear();
    for (Category *category : this->category_list) {
        QPixmap pixmap(ICON_SIZE, ICON_SIZE);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->list_widget->addItem(new QListWidgetItem(QIcon(pixmap), QString(category->getName().c_str())));
    }
}

void CategoryDialog::on_item_click() {
    CategoryEditDialog *category_edit_dialog = new CategoryEditDialog(this);
    category_edit_dialog->show();
}

void CategoryDialog::on_button_cancel_click() {
    this->close();
    delete this;
}

void CategoryDialog::on_button_add_click() {
    CategoryEditDialog *category_edit_dialog = new CategoryEditDialog(this);
    category_edit_dialog->show();
}

vector<Category*> CategoryDialog::getCategoryList() {
    return this->category_list;
}

QListWidget* CategoryDialog::getListWidget() {
    return this->list_widget;
}

PManager* CategoryDialog::getPManager() {
    return this->pm;
}
View* CategoryDialog::getParentView() {
    return this->parent;
}
