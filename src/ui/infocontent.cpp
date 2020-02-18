#include <iostream>
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
    mainLayout->setContentsMargins(5, 5, 5, 5);

    QHBoxLayout* infoLayout = new QHBoxLayout(this);

    QLabel* studioIcon = new QLabel(this);
    studioIcon->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    studioIcon->setPixmap(QPixmap(":/resources/icon-studio-live-32.png"));
    infoLayout->addWidget(studioIcon, 0, Qt::AlignTop);

    QLabel* infoLabel = new QLabel(this);
    infoLabel->setWordWrap(true);
    infoLabel->setText("Rokoko Studio Live for Maya v1.0.0\n\nDeveloped by Rokoko Electronics Aps");
    infoLayout->addWidget(infoLabel, 0, Qt::AlignLeft);
    mainLayout->addLayout(infoLayout);

    QHBoxLayout* licenseWebsiteLayout = new QHBoxLayout(this);
    mainLayout->addLayout(licenseWebsiteLayout);

    // license
    QPushButton* licenseBtn = new QPushButton("License", this);
    connect(licenseBtn, &QPushButton::clicked, [](){
        std::cout << "Open LICENSE.md\n";
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
        std::cout << "Open docs\n";
    });
    mainLayout->addWidget(docsBtn);

    // forums
    QPushButton* fodumsBtn = new QPushButton("Join our forums", this);
    connect(fodumsBtn, &QPushButton::clicked, [](){
        std::cout << "Open forums\n";
    });
    mainLayout->addWidget(fodumsBtn);

}

void InfoContent::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

//    QPainter p(this);
//    p.fillRect(rect(), QColor(99, 99, 99) );

}
