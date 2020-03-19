#include "updatercontent.h"
#include "constants.h"
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>


UpdaterContent::UpdaterContent(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1 ,1);
    mainLayout->setSpacing(1);

    QHBoxLayout* updateButtons = new QHBoxLayout(this);
    updateButtons->setContentsMargins(1, 1, 1, 1);
    updateButtons->setSpacing(1);
    QPushButton* updateBtn = new QPushButton("Check now for Update", this);
    updateBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton* retryButton = new QPushButton(QIcon(":/resources/update.svg"), "", this);
    retryButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    updateButtons->addWidget(updateBtn);
    updateButtons->addWidget(retryButton);

    QFormLayout* versionFormLayout = new QFormLayout(this);
    QHBoxLayout* versionLayout = new QHBoxLayout(this);
    versionLayout->setContentsMargins(1, 1, 1, 1);
    versionLayout->setSpacing(1);
    QComboBox* versionsComboBox = new QComboBox(this);
    versionsComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    versionLayout->addWidget(versionsComboBox);
    QPushButton* releaseNotesBtn = new QPushButton(QIcon(":/resources/paper.svg"), "", this);
    releaseNotesBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    versionLayout->addWidget(releaseNotesBtn);
    versionFormLayout->addRow("Version:", versionLayout);

    QPushButton* installSelectedVersoinBtn = new QPushButton("Install Selected Version", this);
    versionFormLayout->addRow(installSelectedVersoinBtn);

    QLabel* versionLabel = new QLabel("1.0.0",this);
    versionFormLayout->addRow("Current version:", versionLabel);

    mainLayout->addLayout(updateButtons);
    mainLayout->addLayout(versionFormLayout);
}
