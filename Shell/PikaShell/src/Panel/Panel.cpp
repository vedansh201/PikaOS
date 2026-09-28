#include "Panel/Panel.hpp"
#include "Core/PikaApplicationManager.hpp"
#include "Launcher/PikaLauncher.hpp"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

Panel::Panel(PikaApplicationManager *appManager, QWidget *parent)
    : QWidget(parent),
      m_appManager(appManager)
{
    setupUi();
}

void Panel::setupUi()
{
    setFixedHeight(50);

    setStyleSheet(R"(
        QWidget {
            background-color: #2B2D3A;
        }

        QPushButton {
            background-color: #3A3D4D;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 6px 12px;
        }

        QPushButton:hover {
            background-color: #4A4D5D;
        }
    )");

    m_layout = new QHBoxLayout(this);

    m_layout->setContentsMargins(10, 5, 10, 5);
    m_layout->setSpacing(8);

    auto *launcherButton = new QPushButton("🚀 Pika", this);

    m_layout->addWidget(launcherButton);

    connect(
        launcherButton,
        &QPushButton::clicked,
        this,
        &Panel::showLauncher
    );
    m_terminalButton = new QPushButton("Terminal", this);
    m_terminalButton->hide();

    m_filesButton = new QPushButton("Files", this);
    m_filesButton->hide();

    m_layout->addWidget(m_terminalButton);
    m_layout->addWidget(m_filesButton);

    connect(
        m_terminalButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (m_appManager)
            {
                m_appManager->toggleTerminal();
            }
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
                m_appManager->toggleFiles();
            }
        }
    );

    m_layout->addStretch();

    m_clock = new QLabel(this);

    m_clock->setStyleSheet(R"(
        color: white;
        font-size: 16px;
        font-weight: bold;
    )");

    m_layout->addWidget(m_clock);

    m_timer = new QTimer(this);

    connect(
        m_timer,
        &QTimer::timeout,
        this,
        &Panel::updateClock
    );

    updateClock();

    m_timer->start(1000);
}

void Panel::updateClock()
{
    m_clock->setText(
        QDateTime::currentDateTime().toString("hh:mm AP")
    );
}

void Panel::updateApplications()
{
    if (!m_appManager)
    {
        return;
    }

    m_terminalButton->setVisible(
        m_appManager->isTerminalOpen()
    );

    m_filesButton->setVisible(
        m_appManager->isFilesOpen()
    );

    updateButtonStates();
}

void Panel::updateActiveApplication()
{
    updateButtonStates();
}

void Panel::updateButtonStates()
{
    if (!m_appManager)
    {
        return;
    }

    if (m_appManager->isTerminalActive())
    {
        m_terminalButton->setStyleSheet(R"(
            QPushButton {
                background-color: #666B80;
                color: white;
                border: 1px solid #8A8FA3;
                border-radius: 6px;
                padding: 6px 12px;
                font-weight: bold;
            }

            QPushButton:hover {
                background-color: #74798E;
            }
        )");
    }
    else if (m_appManager->isTerminalVisible())
    {
        m_terminalButton->setStyleSheet(R"(
            QPushButton {
                background-color: #3A3D4D;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #4A4D5D;
            }
        )");
    }
    else
    {
        m_terminalButton->setStyleSheet(R"(
            QPushButton {
                background-color: #30333F;
                color: #AEB1BC;
                border: none;
                border-radius: 6px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #3F424F;
            }
        )");
    }

    if (m_appManager->isFilesActive())
    {
        m_filesButton->setStyleSheet(R"(
            QPushButton {
                background-color: #666B80;
                color: white;
                border: 1px solid #8A8FA3;
                border-radius: 6px;
                padding: 6px 12px;
                font-weight: bold;
            }

            QPushButton:hover {
                background-color: #74798E;
            }
        )");
    }
    else if (m_appManager->isFilesVisible())
    {
        m_filesButton->setStyleSheet(R"(
            QPushButton {
                background-color: #3A3D4D;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #4A4D5D;
            }
        )");
    }
    else
    {
        m_filesButton->setStyleSheet(R"(
            QPushButton {
                background-color: #30333F;
                color: #AEB1BC;
                border: none;
                border-radius: 6px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #3F424F;
            }
        )");
    }
}

void Panel::showLauncher()
{
    if (!m_launcher)
    {
        m_launcher = new PikaLauncher(m_appManager);

        m_launcher->setAttribute(
            Qt::WA_DeleteOnClose
        );

        connect(
            m_launcher,
            &QObject::destroyed,
            this,
            [this]()
            {
                m_launcher = nullptr;
            }
        );
    }

    QPoint position = mapToGlobal(
        QPoint(
            10,
            -m_launcher->sizeHint().height() - 5
        )
    );

    m_launcher->adjustSize();
    m_launcher->move(position);
    m_launcher->show();
    m_launcher->raise();
    m_launcher->activateWindow();
}