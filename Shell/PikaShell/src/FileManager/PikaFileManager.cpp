#include "FileManager/PikaFileManager.hpp"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QUrl>

PikaFileManager::PikaFileManager(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Pika Files");
    resize(1000, 650);

    // -----------------------------
    // Navigation bar
    // -----------------------------

    m_backButton = new QPushButton("←", this);
    m_forwardButton = new QPushButton("→", this);
    m_upButton = new QPushButton("↑", this);

    m_backButton->setFixedWidth(40);
    m_forwardButton->setFixedWidth(40);
    m_upButton->setFixedWidth(40);

    m_pathBar = new QLineEdit(this);
    m_pathBar->setReadOnly(true);

    auto *navigationLayout = new QHBoxLayout;

    navigationLayout->addWidget(m_backButton);
    navigationLayout->addWidget(m_forwardButton);
    navigationLayout->addWidget(m_upButton);
    navigationLayout->addWidget(m_pathBar);

    // -----------------------------
    // File list
    // -----------------------------

    m_fileList = new QListWidget(this);

    m_fileList->setViewMode(QListView::ListMode);
    m_fileList->setIconSize(QSize(32, 32));
    m_fileList->setSpacing(4);

    // -----------------------------
    // Main layout
    // -----------------------------

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 10, 10, 10);
    layout->addLayout(navigationLayout);
    layout->addWidget(m_fileList);

    // -----------------------------
    // Connections
    // -----------------------------

    connect(
        m_fileList,
        &QListWidget::itemDoubleClicked,
        this,
        [this](QListWidgetItem *)
        {
            openItem();
        }
    );

    connect(
        m_backButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goBack
    );

    connect(
        m_forwardButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goForward
    );

    connect(
        m_upButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goUp
    );

    // -----------------------------
    // Start at Home
    // -----------------------------

    loadDirectory(QDir::homePath());
}


// --------------------------------------------------
// Navigate to directory
// --------------------------------------------------

void PikaFileManager::navigateTo(const QString &path)
{
    if (path == m_currentPath)
        return;

    if (!m_currentPath.isEmpty())
    {
        m_backHistory.append(m_currentPath);
    }

    m_forwardHistory.clear();

    loadDirectory(path);
}


// --------------------------------------------------
// Load directory
// --------------------------------------------------

void PikaFileManager::loadDirectory(const QString &path)
{
    QDir directory(path);

    if (!directory.exists())
        return;

    m_currentPath = directory.absolutePath();

    m_pathBar->setText(m_currentPath);

    m_fileList->clear();

    QFileIconProvider iconProvider;

    QFileInfoList entries =
        directory.entryInfoList(
            QDir::AllEntries |
            QDir::NoDotAndDotDot,
            QDir::DirsFirst |
            QDir::Name
        );

    for (const QFileInfo &entry : entries)
    {
        auto *item = new QListWidgetItem;

        item->setText(entry.fileName());

        item->setIcon(
            iconProvider.icon(entry)
        );

        item->setData(
            Qt::UserRole,
            entry.absoluteFilePath()
        );

        m_fileList->addItem(item);
    }

    m_backButton->setEnabled(
        !m_backHistory.isEmpty()
    );

    m_forwardButton->setEnabled(
        !m_forwardHistory.isEmpty()
    );
}


// --------------------------------------------------
// Open file/folder
// --------------------------------------------------

void PikaFileManager::openItem()
{
    auto *item = m_fileList->currentItem();
    qDebug() << "Pika Files: openItem called";

    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();
    qDebug() << "Selected path:" << path;

    QFileInfo info(path);

    // Folder
    if (info.isDir())
    {
        navigateTo(path);
        return;
    }

    // File
    if (info.isFile())
    {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(path)
        );
    }
}


// --------------------------------------------------
// Go up
// --------------------------------------------------

void PikaFileManager::goUp()
{
    QDir directory(m_currentPath);

    if (!directory.cdUp())
        return;

    navigateTo(
        directory.absolutePath()
    );
}


// --------------------------------------------------
// Back
// --------------------------------------------------

void PikaFileManager::goBack()
{
    if (m_backHistory.isEmpty())
        return;

    m_forwardHistory.append(m_currentPath);

    QString previousPath =
        m_backHistory.takeLast();

    loadDirectory(previousPath);
}


// --------------------------------------------------
// Forward
// --------------------------------------------------

void PikaFileManager::goForward()
{
    if (m_forwardHistory.isEmpty())
        return;

    m_backHistory.append(m_currentPath);

    QString nextPath =
        m_forwardHistory.takeLast();

    loadDirectory(nextPath);
}