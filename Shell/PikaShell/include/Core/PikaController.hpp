#pragma once

#include <QObject>
#include <QTimer>
#include <QPoint>

class PikachuWidget;

enum class PikachuState
{
    Idle,
    Dragged,
    Walking,
    Sleeping,
    Interacting
};

enum class FacingDirection
{
    Left,
    Right
};

class PikaController : public QObject
{


public:
    explicit PikaController(QObject *parent = nullptr);

    void setWidget(PikachuWidget *widget);

    PikachuState state() const;
    void setState(PikachuState state);

    void startWalking();
    void setHomePosition(const QPoint &pos);

    FacingDirection facingDirection() const
    {
        return m_facing;
    }

private:
    PikachuWidget *m_widget = nullptr;

    PikachuState m_state = PikachuState::Idle;
    FacingDirection m_facing = FacingDirection::Right;

    QTimer *m_idleTimer = nullptr;
    QTimer *m_walkTimer = nullptr;
    QTimer *m_turnTimer = nullptr;
    QPoint m_homePosition;
    QPoint m_walkTarget;

    bool m_homeInitialized = false;

    int m_idleDirection = 1;
};