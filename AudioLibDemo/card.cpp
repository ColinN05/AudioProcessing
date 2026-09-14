#include "card.h"

#include <QVBoxLayout>
#include <QLabel>

Card::Card(const QString& title, QWidget *parent)
    : QFrame(parent)
{
    setObjectName("card");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(8);

    QWidget* titleContainer = new QWidget;
    layout->addWidget(titleContainer);
    auto* titleLayout = new QHBoxLayout(titleContainer);
    auto* label = new QLabel(title);
    titleLayout->addWidget(label);
    titleLayout->addStretch();
    m_CheckBox = new QCheckBox;
    titleLayout->addWidget(m_CheckBox);

    auto* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);


    label->setStyleSheet(R"(
        QLabel {
            font-family: "Segoe UI";
            font-size: 15px;
            color: #FFFFFF;
        }
    )");

    setStyleSheet(R"(
        QFrame#card {
            background-color: gray;
            border: 1px solid #3c3b3b;
            border-radius: 3px;
        }
    )");

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void Card::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}