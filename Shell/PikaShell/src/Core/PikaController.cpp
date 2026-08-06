#include "Core/PikaController.hpp"

#include <QTimer>
#include <QPoint>
#include "Widgets/PikachuWidget.hpp"

#include <QPoint>

PikaController::PikaController(QObject *parent)
    : QObject(parent)
{
    m_idleTimer = new QTimer(this);

    connect(m_idleTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_widget)
            return;

        if (!m_homeInitialized)
        {
            m_homePosition = m_widget->pos();
            m_homeInitialized = true;
        }

        QPoint pos = m_widget->pos();
        pos.setY(pos.y() + m_idleDirection);

        m_widget->move(pos);

        if (pos.y() >= m_homePosition.y() + 5)
            m_idleDirection = -1;

        if (pos.y() <= m_homePosition.y() - 5)
            m_idleDirection = 1;
    });

    m_idleTimer->start(50);
}

void PikaController::setWidget(PikachuWidget *widget)
{
    m_widget = widget;
}

PikachuState PikaController::state() const
{
    return m_state;
}

void PikaController::setState(PikachuState state)
{
    m_state = state;
}
void PikaController::setHomePosition(const QPoint &pos)
{
    m_homePosition = pos;
    m_homeInitialized = true;
}