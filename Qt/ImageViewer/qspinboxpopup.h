#ifndef QSPINBOXPOPUP_H
#define QSPINBOXPOPUP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>

class QSpinBoxPopUp : public QWidget
{
    Q_OBJECT
public:
    explicit QSpinBoxPopUp(QWidget *parent = 0, const int& nboxes=1, const QString title="");

    virtual ~QSpinBoxPopUp();

    const int& GetNboxes() { return nBoxes; }

    double GetValueBox(const int& ibox);

    void SetValueBox(const int& ibox, const double& val);

    void SetMaxValueBox(const int& ibox, const double& val);

    void SetStepSizeBox(const int& ibox, const double& step);

    void SetTextBox(const int& ibox, const QString& text);

    const QPushButton* GetPushButton() { return btn1; }

signals:

public slots:

private:

    bool CheckRange(const int& ibox) { return (ibox >= nBoxes); }

    QWidget *wdg;
    QVBoxLayout *vlay;
    QPushButton *btn1;
    int nBoxes;
    QSpinBox **boxes;


};

#endif // QSPINBOXPOPUP_H
