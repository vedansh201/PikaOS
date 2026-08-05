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

class PikaController : public QObject
{
    Q_OBJECT

public:
    explicit PikaController(QObject *parent = nullptr);

    void setWidget(PikachuWidget *widget);

    PikachuState state() const;
    void setState(PikachuState state);

private:
    PikachuWidget *m_widget = nullptr;
    PikachuState m_state = PikachuState::Idle;
};