#include "Panel/Panel.hpp"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

Panel::Panel(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void Panel::setupUi()
{
    setFixedHeight(50);

    setStyleSheet(R"(
        background-color: #2B2D3A;
    )");

    auto *layout = new QHBoxLayout(this);

    layout->setContentsMargins(10, 5, 10, 5);

    auto *launcherButton = new QPushButton("🚀 Pika");

    
    m_clock = new QLabel(this);

    m_clock->setStyleSheet(R"(
         color: white;
         font-size: 16px;
         font-weight: bold;
     )");

    layout->addWidget(launcherButton);
    layout->addStretch();
    layout->addWidget(m_clock);

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout,
             this, &Panel::updateClock);

    updateClock();
    m_timer->start(1000);

}


void Panel::updateClock()
{
    m_clock->setText(
        QDateTime::currentDateTime().toString("hh:mm AP")
    );
}


