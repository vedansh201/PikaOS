#include "Core/PikaApplicationManager.hpp"
#include "Terminal/PikaTerminal.hpp"
#include "FileManager/PikaFileManager.hpp"

#include <QApplication>
#include <QWidget>

PikaApplicationManager::PikaApplicationManager(QObject *parent)
    : QObject(parent)
{
    connect(
        qApp,
        &QApplication::focusChanged,
        this,
        &PikaApplicationManager::updateActiveApplication
    );
}

void PikaApplicationManager::openTerminal()
{
    if (m_terminal == nullptr)
    {
        m_terminal = new PikaTerminal();

        m_terminal->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(
            m_terminal,
            &QObject::destroyed,
            this,
            [this]()
            {
                m_terminal = nullptr;

                if (m_activeApplication == nullptr)
                {
                    emit activeApplicationChanged();
                }

                emit applicationsChanged();
            }
        );
    }

    m_terminal->show();
    m_terminal->raise();
    m_terminal->activateWindow();
    m_terminal->setFocus();

    emit applicationsChanged();
}

void PikaApplicationManager::openFiles()
{
    if (m_fileManager == nullptr)
    {
        m_fileManager = new PikaFileManager();

        m_fileManager->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(
            m_fileManager,
            &QObject::destroyed,
            this,
            [this]()
            {
                m_fileManager = nullptr;

                if (m_activeApplication == nullptr)
                {
                    emit activeApplicationChanged();
                }

                emit applicationsChanged();
            }
        );
    }

    m_fileManager->show();
    m_fileManager->raise();
    m_fileManager->activateWindow();
    m_fileManager->setFocus();

    emit applicationsChanged();
}

void PikaApplicationManager::toggleTerminal()
{
    if (m_terminal == nullptr)
    {
        openTerminal();
        return;
    }

    if (m_terminal->isVisible())
    {
        m_terminal->hide();

        if (m_activeApplication == m_terminal)
        {
            m_activeApplication = nullptr;
            emit activeApplicationChanged();
        }
    }
    else
    {
        m_terminal->show();
        m_terminal->raise();
        m_terminal->activateWindow();
        m_terminal->setFocus();
    }

    emit applicationsChanged();
}

void PikaApplicationManager::toggleFiles()
{
    if (m_fileManager == nullptr)
    {
        openFiles();
        return;
    }

    if (m_fileManager->isVisible())
    {
        m_fileManager->hide();

        if (m_activeApplication == m_fileManager)
        {
            m_activeApplication = nullptr;
            emit activeApplicationChanged();
        }
    }
    else
    {
        m_fileManager->show();
        m_fileManager->raise();
        m_fileManager->activateWindow();
        m_fileManager->setFocus();
    }

    emit applicationsChanged();
}

bool PikaApplicationManager::isTerminalOpen() const
{
    return m_terminal != nullptr;
}

bool PikaApplicationManager::isFilesOpen() const
{
    return m_fileManager != nullptr;
}

bool PikaApplicationManager::isTerminalVisible() const
{
    return m_terminal != nullptr && m_terminal->isVisible();
}

bool PikaApplicationManager::isFilesVisible() const
{
    return m_fileManager != nullptr && m_fileManager->isVisible();
}

bool PikaApplicationManager::isTerminalActive() const
{
    return m_activeApplication == m_terminal;
}

bool PikaApplicationManager::isFilesActive() const
{
    return m_activeApplication == m_fileManager;
}

void PikaApplicationManager::updateActiveApplication(
    QWidget *oldWidget,
    QWidget *newWidget)
{
    Q_UNUSED(oldWidget);

    QWidget *newWindow = nullptr;

    if (newWidget != nullptr)
    {
        newWindow = newWidget->window();
    }

    QWidget *newActiveApplication = nullptr;

    if (m_terminal != nullptr &&
        newWindow == m_terminal)
    {
        newActiveApplication = m_terminal;
    }
    else if (m_fileManager != nullptr &&
             newWindow == m_fileManager)
    {
        newActiveApplication = m_fileManager;
    }

    if (m_activeApplication == newActiveApplication)
    {
        return;
    }

    m_activeApplication = newActiveApplication;

    emit activeApplicationChanged();
}