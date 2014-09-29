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
    explicit popup_window1(QWidget *parent = 0, const QString& text1="", const QString& text2="");
    QPushButton *btn1;

    double GetValueBox1() { return box1->value(); }

    double GetValueBox2() { return box2->value(); }

    void   SetValueBox2(const int& val) { box2->setValue(val); }

    void SetStepSizeBox1(const int& step) { box1->setSingleStep(step); }

    void SetStepSizeBox2(const int& step) { box2->setSingleStep(step); }

signals:

public slots:


private:
    QWidget *window;

    QSpinBox *box1;
    QSpinBox *box2;

};

#endif // POPUP_WINDOW1_H
