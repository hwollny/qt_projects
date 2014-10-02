#include "qlabelpopup.h"

QlabelPopUp::QlabelPopUp(QWidget *parent) :
    QWidget(parent), wdg(0), vlay(0), imageLabel(0), scrollArea(0), pwidth(500), pheight(400)
{

    wdg = new QWidget(this);
    vlay = new QVBoxLayout(wdg);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    vlay->addWidget(scrollArea);
    wdg->setLayout(vlay);
    //wdg->adjustSize();
    setWindowTitle(tr("Image Viewer 2"));
    resize(pwidth, pheight);

    qDebug() << "HERE in constructor QlabelPopUp";
}

void QlabelPopUp::SetImage(const QPixmap& pixmap)
{
    imageLabel->setPixmap(pixmap);
    imageLabel->adjustSize();
    scrollArea->setWidgetResizable(true);
}

void QlabelPopUp::AdjustSize(const QSize& size)
{
    int pheight_t = pheight;
    int pwidth_t = pwidth;
    bool change_s = false;
    if(size.height() < pheight_t) pheight_t = size.height();
    else change_s = true;
    if(size.width() < pwidth_t) pwidth_t = size.width();
    else change_s = true;

    //if(change_s)
    if(pheight_t!=pheight || pwidth_t != pwidth) resize(pwidth_t, pheight_t);

}
