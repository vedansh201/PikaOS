#include "Launcher/PikaLauncher.hpp"
#include "Core/PikaApplicationManager.hpp"

#include <QVBoxLayout>
#include <QPushButton>

PikaLauncher::PikaLauncher(
    PikaApplicationManager *appManager,
    QWidget *parent
)
    : QWidget(parent),
      m_appManager(appManager)
{
    setupUi();
}

void PikaLauncher::setupUi()
{
    setWindowTitle("Pika");

    setWindowFlags(Qt::Popup);

    setFixedWidth(240);

    setStyleSheet(R"(
        QWidget {
            background-color: #2B2D3A;
        }

        QPushButton {
            background-color: #3A3D4D;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px;
            text-align: left;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #4A4D5D;
        }

        QPushButton:pressed {
            background-color: #666B80;
        }
    )");

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_terminalButton = new QPushButton("Terminal", this);
    m_filesButton = new QPushButton("Files", this);

    layout->addWidget(m_terminalButton);
    layout->addWidget(m_filesButton);

    connect(
        m_terminalButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (m_appManager)
            {
                m_appManager->openTerminal();
            }

            close();
        }
    );

    connect(
        m_filesButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (m_appManager)
            {
                m_appManager->openFiles();
            }

            close();
        }
    );
}