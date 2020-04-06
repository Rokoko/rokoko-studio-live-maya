#include "constants.h"
#include "utils.h"
#include "infocontent.h"
#include <QDesktopServices>
#include <QUrl>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>


InfoContent::InfoContent(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding));
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(1);

    QHBoxLayout* infoLayout = new QHBoxLayout(this);

    QLabel* studioIcon = new QLabel(this);
    studioIcon->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    studioIcon->setPixmap(QPixmap(":/resources/icon-studio-live-32.png"));
    infoLayout->addWidget(studioIcon, 0, Qt::AlignTop);

    QLabel* infoLabel = new QLabel(this);
    infoLabel->setWordWrap(true);
    QString infoText = QString("Rokoko Studio Live for Maya v%1.%2.%3\n\nDeveloped by Rokoko Electronics Aps").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH);
    infoLabel->setText(infoText);
    infoLayout->addWidget(infoLabel, 0, Qt::AlignLeft);
    mainLayout->addLayout(infoLayout);

    QHBoxLayout* licenseWebsiteLayout = new QHBoxLayout(this);
    licenseWebsiteLayout->setContentsMargins(0, 1, 0, 0);
    licenseWebsiteLayout->setSpacing(1);
    mainLayout->addLayout(licenseWebsiteLayout);

    // license
    QPushButton* licenseBtn = new QPushButton("License", this);
    connect(licenseBtn, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://github.com/RokokoElectronics/rokoko-studio-live-maya/blob/master/LICENSE.md"));
    });
    licenseWebsiteLayout->addWidget(licenseBtn);

    // website
    QPushButton* websiteBtn = new QPushButton("Website", this);
    connect(websiteBtn, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://www.rokoko.com"));
    });
    licenseWebsiteLayout->addWidget(websiteBtn);

    // docs
    QPushButton* docsBtn = new QPushButton("Documentation", this);
    connect(docsBtn, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://rokoko.freshdesk.com/support/solutions/folders/47000766113"));
    });
    mainLayout->addWidget(docsBtn);

    // forums
    QPushButton* fodumsBtn = new QPushButton("Join our forums", this);
    connect(fodumsBtn, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://rokoko.freshdesk.com/support/discussions/forums/47000400064"));
    });
    mainLayout->addWidget(fodumsBtn);

}

void InfoContent::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

//    QPainter p(this);
//    p.fillRect(rect(), QColor(99, 99, 99) );

}
