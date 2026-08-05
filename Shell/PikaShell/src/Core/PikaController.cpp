#include "Core/PikaController.hpp"

PikaController::PikaController(QObject *parent)
    : QObject(parent)
{
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