#include "Core/PikaWindow.hpp"
#include "Panel/Panel.hpp"
#include "Widgets/PikachuWidget.hpp"
#include "Core/Desktop.hpp"
#include <QVBoxLayout>
#include "Core/PikaWindow.hpp"
#include "Core/PikaApplicationManager.hpp"
#include "Core/PikaCommandParser.hpp"
PikaWindow::PikaWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

/**void PikaWindow::setupUi()
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

    // Temporary test
    QWidget *test = new QWidget(this);

    layout->addWidget(test);
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

    Desktop *desktop = new Desktop(this);

    layout->addWidget(desktop);
}
**/
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
    Panel *panel = new Panel(this);

    layout->addWidget(desktop);
    layout->addWidget(panel);
}