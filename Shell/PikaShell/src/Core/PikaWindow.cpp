#include "Core/PikaWindow.hpp"
#include "Panel/Panel.hpp"
#include "Widgets/PikachuWidget.hpp"
#include "Core/Desktop.hpp"
#include <QVBoxLayout>

PikaWindow::PikaWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void PikaWindow::setupUi()
{
    setWindowTitle("PikaShell");
    resize(1280, 720);

    setStyleSheet(R"(
        background-color: #1E1F29;
    )");

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    Desktop *desktop = new Desktop(this);

    m_panel = new Panel(this);

    layout->addWidget(desktop);
    layout->addWidget(m_panel);
}