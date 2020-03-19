#ifndef RECORDBUTTON_H
#define RECORDBUTTON_H

#include "ui/button.h"
#include <QEvent>
#include <QFontMetrics>
#include <QTimer>


class RecordButton : public Button
{
    Q_OBJECT
public:
    RecordButton(QWidget* parent=nullptr);
protected:
    void changeEvent(QEvent*) override;
    void paintEvent(QPaintEvent *) override;
private:
    void onHearbeat();
    QFont recordedFramesFont;
    QTimer updater;
};

#endif // RECORDBUTTON_H
