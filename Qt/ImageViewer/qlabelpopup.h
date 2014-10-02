#ifndef QLABELPOPUP_H
#define QLABELPOPUP_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>

class QlabelPopUp : public QWidget
{
    Q_OBJECT
public:
    explicit QlabelPopUp(QWidget *parent = 0);

    void SetImage(const QPixmap& pixmap);

    void AdjustSize(const QSize& size);
signals:

public slots:

private:

    QWidget *wdg;
    QVBoxLayout *vlay;
    QLabel *imageLabel;
    QScrollArea *scrollArea;

    int pwidth, pheight;
};

#endif // QLABELPOPUP_H
