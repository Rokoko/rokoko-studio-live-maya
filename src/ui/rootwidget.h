#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class RootWidget : public QWidget
{
    Q_OBJECT

public:
    RootWidget(QWidget *parent = nullptr);
    ~RootWidget();
};
#endif // WIDGET_H
