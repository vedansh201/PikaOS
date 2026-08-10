
#include "Core/PikaController.hpp"

#include <QDebug>
#include <QTimer>
#include <QPoint>
#include <QRandomGenerator>

#include "Widgets/PikachuWidget.hpp"

#include <algorithm>
#include <cmath>

PikaController::PikaController(QObject *parent)
    : QObject(parent)
{
    // -------------------------
    // Idle timer
    // -------------------------

    m_idleTimer = new QTimer(this);

    connect(m_idleTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_widget)
            return;

        if (m_state != PikachuState::Idle)
            return;

        if (!m_homeInitialized)
        {
            m_homePosition = m_widget->pos();
            m_homeInitialized = true;
        }

        QPoint pos = m_widget->pos();

        // Idle bobbing
        pos.setY(pos.y() + m_idleDirection);

        m_widget->move(pos);

        // Reverse at the top
        if (pos.y() >= m_homePosition.y() + 5)
            m_idleDirection = -1;

        // Reverse at the bottom
        if (pos.y() <= m_homePosition.y() - 5)
            m_idleDirection = 1;
    });

    m_idleTimer->start(50);


    // -------------------------
    // Walking timer
    // -------------------------

    m_walkTimer = new QTimer(this);
    m_walkTimer->setInterval(16);


    // -------------------------
    // Turn / pause timer
    // -------------------------

    m_turnTimer = new QTimer(this);
    m_turnTimer->setSingleShot(true);

    connect(m_turnTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_widget)
            return;

        // After the pause, choose a new target.
        startWalking();
    });


    // -------------------------
    // Start walking after 3 sec
    // -------------------------

    QTimer::singleShot(3000, this, [this]()
    {
        startWalking();
    });


    // -------------------------
    // Walking logic
    // -------------------------

    connect(m_walkTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_widget)
            return;

        QWidget *desktop = m_widget->parentWidget();

        if (!desktop)
            return;

        QPoint pos = m_widget->pos();

        // Move toward target
        if (pos.x() < m_walkTarget.x())
            pos.rx() += 2;
        else if (pos.x() > m_walkTarget.x())
            pos.rx() -= 2;


        // -------------------------
        // Desktop boundaries
        // -------------------------

        int leftEdge = 0;
        int rightEdge = desktop->width() - m_widget->width();


        // Hit LEFT edge
        if (pos.x() <= leftEdge)
        {
            pos.setX(leftEdge);

            m_widget->move(pos);
            setHomePosition(pos);

            // Face right
            m_facing = FacingDirection::Right;
            m_widget->setFacingLeft(false);

            // Stop and pause
            m_walkTimer->stop();
            m_turnTimer->start(800);

            return;
        }


        // Hit RIGHT edge
        if (pos.x() >= rightEdge)
        {
            pos.setX(rightEdge);

            m_widget->move(pos);
            setHomePosition(pos);

            // Face left
            m_facing = FacingDirection::Left;
            m_widget->setFacingLeft(true);

            // Stop and pause
            m_walkTimer->stop();
            m_turnTimer->start(800);

            return;
        }


        // -------------------------
        // Normal movement
        // -------------------------

        m_widget->move(pos);
        setHomePosition(pos);


        // -------------------------
        // Reached random target
        // -------------------------
        if (std::abs(pos.x() - m_walkTarget.x()) <= 2)
        {
            m_walkTimer->stop();

            setHomePosition(pos);

            m_idleDirection = 1;

    // Brief pause before choosing another destination
            int idleTime = QRandomGenerator::global()->bounded(500, 2001);

            m_turnTimer->start(idleTime);
        }    
    });
}


// ============================================================
// Widget
// ============================================================

void PikaController::setWidget(PikachuWidget *widget)
{
    m_widget = widget;
}


// ============================================================
// State
// ============================================================

PikachuState PikaController::state() const
{
    return m_state;
}

void PikaController::setState(PikachuState state)
{
    m_state = state;

    if (state == PikachuState::Interacting)
    {
        if (m_walkTimer)
            m_walkTimer->stop();

        if (m_turnTimer)
            m_turnTimer->stop();
    }
}
// ============================================================
// Home position
// ============================================================

void PikaController::setHomePosition(const QPoint &pos)
{
    m_homePosition = pos;
    m_homeInitialized = true;
}


// ============================================================
// Start walking
// ============================================================

void PikaController::startWalking()
{
    if (!m_widget)
        return;

    QWidget *desktop = m_widget->parentWidget();

    if (!desktop)
        return;


    int leftEdge = 0;
    int rightEdge = desktop->width() - m_widget->width();

    int currentX = m_widget->x();


    // -------------------------
    // Choose random target
    // -------------------------

    int minTarget = std::max(leftEdge, currentX - 150);
    int maxTarget = std::min(rightEdge, currentX + 150);

    if (minTarget >= maxTarget)
        return;


    int targetX = QRandomGenerator::global()->bounded(
        minTarget,
        maxTarget + 1
    );


    // -------------------------
    // Determine direction
    // -------------------------

    if (targetX > currentX)
    {
        m_facing = FacingDirection::Right;
        m_widget->setFacingLeft(false);
    }
    else
    {
        m_facing = FacingDirection::Left;
        m_widget->setFacingLeft(true);
    }


    // -------------------------
    // Set target
    // -------------------------

    m_walkTarget = QPoint(
        targetX,
        m_widget->y()
    );


    qDebug() << "Walking from"
             << currentX
             << "to"
             << targetX;


    // -------------------------
    // Start walking
    // -------------------------

    m_state = PikachuState::Walking;

    m_walkTimer->start();
}
