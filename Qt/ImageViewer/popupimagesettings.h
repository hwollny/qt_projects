#ifndef POPUPIMAGESETTINGS_H
#define POPUPIMAGESETTINGS_H

#include <QDialog>

namespace Ui {
class PopUpImageSettings;
}

class PopUpImageSettings : public QDialog
{
    Q_OBJECT

public:
    explicit PopUpImageSettings(QWidget *parent = 0);
    ~PopUpImageSettings();

private:
    Ui::PopUpImageSettings *ui;

};

#endif // POPUPIMAGESETTINGS_H
