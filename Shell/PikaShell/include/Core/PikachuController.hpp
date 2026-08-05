#pragma once

#include <QObject>

class PikachuWidget;

enum class PikachuState
{
    Idle,
    Dragged,
    Walking,
    Sleeping
};

class PikachuController : public QObject
{
    Q_OBJECT

public:
    explicit PikachuController(QObject *parent = nullptr);

    void setWidget(PikachuWidget *widget);

    PikachuState state() const;

    void setState(PikachuState state);

private:
    PikachuWidget *m_widget = nullptr;
    PikachuState m_state = PikachuState::Idle;
};

class PikachuController;

public:
    void setController(PikachuController *controller);

private:
    PikachuController *m_controller = nullptr;