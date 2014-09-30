#include "qspinboxpopup.h"

QSpinBoxPopUp::QSpinBoxPopUp(QWidget *parent, const int& nboxes, const QString title) :
    QWidget(parent), btn1(0), nBoxes(nboxes), boxes(0)
{
    Q_ASSERT_X(nboxes>=1,"QSpinBoxPopUp::QSpinBoxPopUp","number of boxes must be at least 1");

    QWidget * wdg = new QWidget(this);
    QVBoxLayout *vlay = new QVBoxLayout(wdg);

    boxes = new QSpinBox*[nboxes];
    for(int ib = 0 ; ib < nboxes ; ib++)
    {
       boxes[ib] =  new QSpinBox();
       vlay->addWidget(boxes[ib]);
    }
    btn1 = new QPushButton("Ok");
    vlay->addWidget(btn1);
    wdg->setLayout(vlay);
    wdg->adjustSize();

    if(title!="") setWindowTitle(title);
}


double QSpinBoxPopUp::GetValueBox(const int& ibox)
{
    Q_ASSERT_X(!CheckRange(ibox),"QSpinBoxPopUp::GetValueBox","Index out of range");

    return boxes[ibox]->value();
}

void QSpinBoxPopUp::SetValueBox(const int& ibox, const double& val)
{
  Q_ASSERT_X(!CheckRange(ibox),"QSpinBoxPopUp::SetValueBox","Index out of range");

  boxes[ibox]->setValue(val);

}

void QSpinBoxPopUp::SetMaxValueBox(const int& ibox, const double& val)
{
  Q_ASSERT_X(!CheckRange(ibox),"QSpinBoxPopUp::SetMaxValueBox","Index out of range");

  boxes[ibox]->setMaximum(val);

}

void QSpinBoxPopUp::SetStepSizeBox(const int& ibox, const double& step)
{
    Q_ASSERT_X(!CheckRange(ibox),"QSpinBoxPopUp::SetStepSizeBox","Index out of range");

    boxes[ibox]->setSingleStep(step);
}

void QSpinBoxPopUp::SetTextBox(const int& ibox, const QString& text)
{
    Q_ASSERT_X(!CheckRange(ibox),"QSpinBoxPopUp::SetTextBox","Index out of range");
    boxes[ibox]->setPrefix(text+":  ");
}
