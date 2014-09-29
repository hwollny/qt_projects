#include "popup_window1.h"

popup_window1::popup_window1(QWidget *parent, const QString &text1, const QString &text2) :
    QWidget(parent)
{
    QWidget * wdg = new QWidget(this);

    QVBoxLayout *vlay = new QVBoxLayout(wdg);

    box1 = new QSpinBox();
    box1->setPrefix(text1+":  ");
    box1->setMinimum(1);
    box1->setMaximum(500);
    box2 = new QSpinBox();
    box2->setPrefix(text2+":  ");
    btn1 = new QPushButton("Ok");
    vlay->addWidget(box1);
    vlay->addWidget(box2);
    vlay->addWidget(btn1);
    wdg->setLayout(vlay);
    wdg->adjustSize();
/*

    vlay->addWidget(btn1);
    QPushButton *btn2 = new QPushButton("btn2");
    vlay->addWidget(btn2);
    QPushButton *btn3 = new QPushButton("btn3");
    vlay->addWidget(btn3);
    wdg->setLayout(vlay);
    */
  //  setCentralWidget(wdg);
}
