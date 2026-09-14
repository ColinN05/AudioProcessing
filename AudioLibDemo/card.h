#pragma once

#include <QCheckBox>
#include <QFrame>

class Card : public QFrame
{
public:
    explicit Card(const QString& title, QWidget *parent = nullptr);
    void addWidget(QWidget *widget);
    const QCheckBox* getCheckBox() const { return m_CheckBox; }
private:
    QCheckBox* m_CheckBox;
};