#ifndef RECORDBUTTON_H
#define RECORDBUTTON_H

#include "ui/button.h"
#include <QEvent>


class RecordButton : public Button
{
    Q_OBJECT
public:
    RecordButton(QWidget* parent=nullptr);
protected:
    void changeEvent(QEvent*) override;
};

#endif // RECORDBUTTON_H
