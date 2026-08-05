#include "Core/PikachuController.hpp"

PikachuController::PikachuController(QObject *parent)
    : QObject(parent)
{
}

void PikachuController::setWidget(PikachuWidget *widget)
{
    m_widget = widget;
}

PikachuState PikachuController::state() const
{
    return m_state;
}

void PikachuController::setState(PikachuState state)
{
    m_state = state;
}

void PikachuWidget::setController(PikachuController *controller)
{
    m_controller = controller;
}