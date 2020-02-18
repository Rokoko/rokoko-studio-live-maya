#include "recordbutton.h"

const ButtonParams recordButtonParams = {
    ":/resources/icon-stop-white-32.png",
    ":/resources/icon-record-32.png",
    "Start Recording",
    "Stop Recording",
    Qt::AlignLeft, 16
};

RecordButton::RecordButton(QWidget* parent)
    : Button(parent, recordButtonParams)
{
    setCheckable(true);

}

void RecordButton::changeEvent(QEvent* e)
{
    int type = e->type();
    if(type == QEvent::EnabledChange) {

        // Reset button on disable
        if(!isEnabled()) {
            setChecked(false);
        }
    }
}
