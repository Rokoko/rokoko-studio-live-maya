#include <iostream>

#include <maya/MGlobal.h>

#include "ui/rootwidget.h"
#include "ui/categoryheader.h"
#include "ui/infocontent.h"
#include "ui/receivercontent.h"


#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QSpacerItem>
#include <QDebug>



RootWidget::RootWidget(QWidget *parent)
    : QWidget(parent)
{
    // apply style
    QFile styleFile(":/resources/style.qss");
    if(styleFile.open(QIODevice::ReadOnly)) {
        QString styleString = styleFile.readAll();
        setStyleSheet(styleString);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(1);

    // Rokoko studio live category
    CategoryHeader* receiverHeader = new CategoryHeader(this, "Rokoko Studio Live");
    mainLayout->addWidget(receiverHeader, 0, Qt::AlignTop);
    ReceiverContent* receiverContent = new ReceiverContent(this);
    mainLayout->addWidget(receiverContent, 0, Qt::AlignTop);
    connect(receiverHeader, &CategoryHeader::collapseStateChanged, [receiverContent](bool bCollapsed) {
        receiverContent->setVisible(!bCollapsed);
    });

    // Command API category
    CategoryHeader* commandAPIHeader = new CategoryHeader(this, "Studio Command API");
    // TODO: implement content
    mainLayout->addWidget(commandAPIHeader, 0, Qt::AlignTop);

    // Updater category
    CategoryHeader* updaterHeader = new CategoryHeader(this, "Updater");
    // TODO: implement content
    mainLayout->addWidget(updaterHeader, 0, Qt::AlignTop);

    // Info category
    CategoryHeader* infoHeader = new CategoryHeader(this, "Info");
    mainLayout->addWidget(infoHeader, 0, Qt::AlignTop);
    InfoContent* infoContentWidget = new InfoContent(this);
    mainLayout->addWidget(infoContentWidget, 1, Qt::AlignTop);
    connect(infoHeader, &CategoryHeader::collapseStateChanged, [infoContentWidget](bool bCollapsed) {
        infoContentWidget->setVisible(!bCollapsed);
    });

    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

}

RootWidget::~RootWidget()
{

}
