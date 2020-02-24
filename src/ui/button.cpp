#include "button.h"
#include <QPainter>


Button::Button(QWidget *parent, ButtonParams params)
    : QPushButton(parent),
      mShouldDrawIcon(true),
      mParams(params)
{
    setText(params.uncheckedStateText);
    connect(this, &QPushButton::toggled, [&](bool checked) {
        setText(checked ? mParams.checkedStateText : mParams.uncheckedStateText);
        update();
    });
}

void Button::setDrawIcon(bool shouldDraw)
{
    mShouldDrawIcon = shouldDraw;
    update();
}

void Button::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);

    QPainter painter(this);

    QString iconPath = isChecked() ? mParams.checkedIconPath : mParams.uncheckedIconPath;

    if(mShouldDrawIcon && !iconPath.isEmpty()) {

        QImage img = QImage(iconPath).scaled(mParams.iconSize, mParams.iconSize);
        int diff = rect().height() - img.height();
        int y = diff / 2;
        int x = 0;
        switch (mParams.iconAlignment) {
            case Qt::AlignRight: {
                x = rect().width() - (img.width() * 1.5);
                break;
            }
            case Qt::AlignCenter: {
                x = rect().center().x() - (img.width() / 2);
                break;
            }
            default: {
                x = img.width() / 2;
                break;
            }
        }
        painter.drawImage(x, y, img);
    }
}
