#include "popupimagesettings.h"
#include "ui_popupimagesettings.h"

PopUpImageSettings::PopUpImageSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopUpImageSettings)
{
    ui->setupUi(this);
}

PopUpImageSettings::~PopUpImageSettings()
{
    delete ui;
}
