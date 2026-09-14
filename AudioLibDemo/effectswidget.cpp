#include "effectswidget.h"
#include "ui_effectswidget.h"

EffectsWidget::EffectsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectsWidget)
{
    ui->setupUi(this);
}

EffectsWidget::~EffectsWidget()
{
    delete ui;
}
