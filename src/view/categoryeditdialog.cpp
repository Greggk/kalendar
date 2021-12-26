#include "categoryeditdialog.h"

CategoryEditDialog::CategoryEditDialog(CategoryDialog *parentDialog, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryEditDialog)
{
    this->parentDialog = parentDialog;
    Category *selected_category = NULL;
    if (this->parentDialog->getListWidget()->selectedItems().size() != 0)
        selected_category = this->parentDialog->getCategoryList()[this->parentDialog->getListWidget()->currentIndex().row()];
    QVBoxLayout *vl = new QVBoxLayout;
    QHBoxLayout *hl = new QHBoxLayout;
    QHBoxLayout *hl2 = new QHBoxLayout;
    button_color = new QPushButton;
    if (selected_category != NULL)
        this->selected_color = QColor(selected_category->getColor().c_str());
    else
        this->selected_color = QColor(DEFAULT_COLOR);
    QPixmap pixmap(ICON_SIZE, ICON_SIZE);
    pixmap.fill(this->selected_color);
    button_color->setIcon(QIcon(pixmap));
    edit_name = new QLineEdit;
    if (selected_category != NULL)
        edit_name->setText(selected_category->getName().c_str());
    else
        edit_name->setPlaceholderText(tr("Type a name for the new category"));
    hl->addWidget(edit_name);
    hl->addWidget(button_color);
    connect(button_color, &QPushButton::clicked, this, &CategoryEditDialog::on_button_color_click);
    QPushButton *button_save = new QPushButton(tr("&Save"));
    QPushButton *button_delete = new QPushButton(tr("&Delete"));
    connect(button_delete, &QPushButton::clicked, this, &CategoryEditDialog::on_button_delete_click);
    connect(button_save, &QPushButton::clicked, this, &CategoryEditDialog::on_button_save_click);
    hl2->addWidget(button_save);
    hl2->addWidget(button_delete);
    vl->addLayout(hl);
    vl->addLayout(hl2);
    this->setModal(true);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("Category Edit Dialog"));
    setLayout(vl);
}

CategoryEditDialog::~CategoryEditDialog() {
    delete this->ui;
}

void CategoryEditDialog::on_button_save_click() {
    if(this->edit_name->text().length() < 3) {
        QMessageBox::critical(this, tr("Error"), tr("The name must have a length greater than 2"), QMessageBox::Ok);
        return;
    }
    Category category(0, this->edit_name->text().toStdString(), this->selected_color.name().toStdString());
    //Create new category
    if ((this->parentDialog->getListWidget()->selectedItems().size() == 0) && (this->parentDialog->getPManager()->add_category(&category))) {
        this->parentDialog->load_categories();
        this->close();
        delete this;
    }else //Change the selected category
    if (this->parentDialog->getPManager()->replace_category(this->parentDialog->getCategoryList()[this->parentDialog->getListWidget()->currentIndex().row()], &category)) {
        this->parentDialog->load_categories();
        this->parentDialog->getParentView()->refresh_events();
        this->close();
        delete this;
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Persistence error. Try with a different name or color."));
    }
}

void CategoryEditDialog::on_button_delete_click() {
    if (this->parentDialog->getListWidget()->selectedItems().size() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("No category selected"), QMessageBox::Ok);
        return;
    }
    QString selected_category = this->parentDialog->getListWidget()->selectedItems().at(0)->text();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm"), QString(tr("Do you want to delete ")) + selected_category + QString("?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (!this->parentDialog->getPManager()->remove_category(this->parentDialog->getCategoryList()[this->parentDialog->getListWidget()->currentIndex().row()]))
            QMessageBox::critical(this, tr("Error"), tr("Persistence error. You can't remove a category used by events. First remove these events and then try again."));
        else {
            this->parentDialog->load_categories();
            this->close();
            delete this;
        }
    }
}

void CategoryEditDialog::on_button_color_click() {
    QColor color = QColorDialog::getColor(QColor(this->selected_color));
    if (color.isValid()) {
        this->selected_color = color;
        QPixmap pixmap(ICON_SIZE, ICON_SIZE);
        pixmap.fill(QColor(this->selected_color));
        button_color->setIcon(QIcon(pixmap));
    }
}
