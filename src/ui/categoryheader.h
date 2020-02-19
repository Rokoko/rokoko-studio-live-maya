#ifndef CATEGORYHEADER_H
#define CATEGORYHEADER_H

#include <QWidget>
#include <QPointer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

class CategoryHeader : public QPushButton
{
    Q_OBJECT
public:
    CategoryHeader(QWidget *parent = nullptr, QString title="Name");

signals:
    void collapseStateChanged(bool);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    bool bCollapsed = false;
};

#endif // CATEGORYHEADER_H
