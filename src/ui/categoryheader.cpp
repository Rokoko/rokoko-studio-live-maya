#include "categoryheader.h"
#include <QPainter>

CategoryHeader::CategoryHeader(QWidget *parent, QString title)
    : QPushButton(parent)
{
    setText(title);
    connect(this, &CategoryHeader::clicked, [this]() {
        bCollapsed = !bCollapsed;
        emit collapseStateChanged(bCollapsed);
        update();
    });
    update();
}

void CategoryHeader::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);

    QPainter painter(this);
    QImage img(":/resources/arrow.svg");
    QMatrix m;
    m.rotate(bCollapsed ? -90 : 0);
    painter.drawImage(0, 0, img.transformed(m));
}
