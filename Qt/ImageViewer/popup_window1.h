#ifndef POPUP_WINDOW1_H
#define POPUP_WINDOW1_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>

class popup_window1 : public QWidget
{
    Q_OBJECT

public:
    explicit popup_window1(QWidget *parent = 0);

    QPushButton *btn1;

    double GetValueBox1() { return box1->value(); }

    double GetValueBox2() { return box2->value(); }

    void   SetValueBox2(const int& val) { box2->setValue(val); }
signals:

public slots:


private:
    QWidget *window;

    QSpinBox *box1;
    QSpinBox *box2;

};

#endif // POPUP_WINDOW1_H
