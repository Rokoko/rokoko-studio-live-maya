#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>


struct ButtonParams {
    ButtonParams(QString _checkedIconPath="",
                  QString _uncheckedIconPath="",
                  QString _uncheckedStateText="...",
                  QString _checkedStateText="",
                  Qt::Alignment _iconAlignment=Qt::AlignLeft,
                  unsigned int _iconSize=16)
        :
          checkedIconPath(_checkedIconPath),
          uncheckedIconPath(_uncheckedIconPath),
          uncheckedStateText(_uncheckedStateText),
          checkedStateText(_checkedStateText),
          iconAlignment(_iconAlignment),
          iconSize(_iconSize) { }

    QString checkedIconPath="";
    QString uncheckedIconPath="";
    QString uncheckedStateText="...";
    QString checkedStateText="...";
    Qt::Alignment iconAlignment = Qt::AlignLeft;
    unsigned int iconSize = 16;
};


class Button : public QPushButton
{
    Q_OBJECT
public:
    Button(QWidget *parent = nullptr, ButtonParams params={});
    void setDrawIcon(bool);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    bool mShouldDrawIcon = true;
    const ButtonParams mParams;
    bool mChecked = false;
};

#endif // BUTTON_H
