#include "constants.h"
#include "recordbutton.h"
#include "recorder.h"

#include <QPainter>

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
    recordedFramesFont = QFont("Consolas", 8);

    updater.setInterval(RECEIVER_FPS * 2);
    connect(&updater, &QTimer::timeout, this, &RecordButton::onHearbeat);
    updater.start();
}

void RecordButton::changeEvent(QEvent* e)
{
    int type = e->type();
    if(type == QEvent::EnabledChange) {

        // Reset button on disable
        if(!isEnabled()) {
            setChecked(false);
        }
        update();
    }
}

void RecordButton::paintEvent(QPaintEvent *event)
{
    Button::paintEvent(event);
    // paint recorded frames num
    QPainter painter(this);
    if(Recorder::get()->recordingEnabled())
    {
        float startTime = Recorder::get()->recordingStartTime();
        unsigned int recordedFrames = Recorder::get()->numFramesAvailable();
        QString text = QString("%1 - %2").arg(startTime).arg(startTime + recordedFrames);
        QFontMetrics m(recordedFramesFont);
        painter.setFont(recordedFramesFont);
        int textWidth = m.horizontalAdvance(text);
        QPoint p = rect().bottomRight() - QPoint(textWidth * 1.5, m.height() / 2);
        painter.drawText(p, text);
    }

}

void RecordButton::onHearbeat()
{
    update();
}
