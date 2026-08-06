#pragma once
#include <QPoint>
#include <QObject>
#include <QTimer>


class PikachuWidget;

enum class PikachuState
{
    Idle,
    Dragged,
    Walking,
    Sleeping
};

class PikaController : public QObject
{
public:
    explicit PikaController(QObject *parent = nullptr);

    void setWidget(PikachuWidget *widget);

    PikachuState state() const;
    void setState(PikachuState state);
    void startWalking();
private:
    PikachuWidget *m_widget = nullptr;
    PikachuState m_state = PikachuState::Idle;
    QTimer *m_walkTimer = nullptr;
    QPoint m_homePosition;
    bool m_homeInitialized = false;
    QPoint m_walkTarget;

private:
    QTimer *m_idleTimer = nullptr;

    int m_idleDirection = 1;

public:
    void setHomePosition(const QPoint &pos);
};