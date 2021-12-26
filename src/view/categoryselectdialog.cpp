#include "categoryselectdialog.h"

#include <QDebug>

CategorySelectDialog::CategorySelectDialog(View *parentView, QString text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectDialog)
{
    this->parent = parentView;
    this->setWindowTitle(tr("Category Selector"));
    this->pm = new PManager;
    this->selected_category = NULL;
    QHBoxLayout *button_layout = new QHBoxLayout;
    QVBoxLayout *main_layout = new QVBoxLayout;
    this->list_categories = new QComboBox;
    load_categories();
    QPushButton *button_ok = new QPushButton(tr("OK"));
    QPushButton *button_cancel = new QPushButton(tr("Cancel"));
    button_cancel->setFixedWidth(50);
    button_ok->setFixedWidth(50);
    connect(button_cancel, &QPushButton::clicked, this, &CategorySelectDialog::on_button_cancel_click);
    connect(button_ok, &QPushButton::clicked, this, &CategorySelectDialog::on_button_ok_click);
    QLabel *message = new QLabel(text);
    main_layout->addWidget(message);
    main_layout->addWidget(this->list_categories);
    button_layout->addWidget(button_ok);
    button_layout->addWidget(button_cancel);
    main_layout->addLayout(button_layout);
    this->setLayout(main_layout);
}

void CategorySelectDialog::load_categories() {
    for (Category *category : this->category_list) delete category;
    this->category_list = this->pm->get_categories();
    for (Category *category : category_list) {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->list_categories->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
}

void CategorySelectDialog::on_button_cancel_click() {
    //this->selected_category = NULL;
    this->close();
    //delete this;
}

void CategorySelectDialog::on_button_ok_click() {
    for (Category *c : this->category_list) {
        if (this->list_categories->currentText().toStdString() == c->getName()) {
            this->selected_category = new Category(*c);
            break;
        }
    }
    this->close();
    //delete this;
}

Category* CategorySelectDialog::getSelectedCategory() {
    return this->selected_category;
}

CategorySelectDialog::~CategorySelectDialog()
{
    delete ui;
    delete pm;
    delete selected_category;
    for (Category *category : this->category_list) delete category;
}
