#ifndef INFOCONTENT_H
#define INFOCONTENT_H

#include <QWidget>

class InfoContent : public QWidget
{
    Q_OBJECT
public:
    InfoContent(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // INFOCONTENT_H
