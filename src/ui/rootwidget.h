#ifndef WIDGET_H
#define WIDGET_H

#include "ui/receivercontent.h"
#include <QWidget>


class RootWidget : public QWidget
{
    Q_OBJECT

public:
    RootWidget(QWidget *parent = nullptr);
    ~RootWidget();
private:
    ReceiverContent* receiverContent=nullptr;
};
#endif // WIDGET_H
