#include "Core/PikaWindow.hpp"
#include "Core/PikaApplicationManager.hpp"
#include "Core/Desktop.hpp"
#include "Panel/Panel.hpp"

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
        QWidget {
            background-color: #1E1F29;
        }
    )");

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_appManager = new PikaApplicationManager(this);

    Desktop *desktop = new Desktop(m_appManager, this);

    m_panel = new Panel(m_appManager, this);

    layout->addWidget(desktop);
    layout->addWidget(m_panel);

        connect(
        m_appManager,
        &PikaApplicationManager::applicationsChanged,
        m_panel,
        &Panel::updateApplications
    );

    connect(
        m_appManager,
        &PikaApplicationManager::activeApplicationChanged,
        m_panel,
        &Panel::updateActiveApplication
    );

    m_panel->updateApplications();
    m_panel->updateActiveApplication();

}