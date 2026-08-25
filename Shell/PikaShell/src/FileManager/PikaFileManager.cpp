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
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>

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
    m_newButton = new QPushButton("+ New", this);

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
    navigationLayout->addWidget(m_newButton);
    // -----------------------------
    // File list
    // -----------------------------

    m_fileList = new QListWidget(this);
   // Sidebar
    m_sidebar = new QListWidget(this);

    m_sidebar->setFixedWidth(190);
    m_sidebar->setIconSize(QSize(24, 24));
    m_sidebar->setSpacing(3);

    auto addSidebarItem =
        [this](const QString &name,
               const QString &path)
   {
         auto *item =
             new QListWidgetItem(name);

        item->setData(
            Qt::UserRole,
            path
        );

        m_sidebar->addItem(item);
   };

    addSidebarItem(
        "🏠  Home",
        QDir::homePath()
   );

    addSidebarItem(
        "🖥  Desktop",
        QDir::homePath() + "/Desktop"
   );

    addSidebarItem(
        "📄  Documents",
        QDir::homePath() + "/Documents"
   );

    addSidebarItem(
        "⬇  Downloads",
        QDir::homePath() + "/Downloads"
   );

    addSidebarItem(
        "🖼  Pictures",
        QDir::homePath() + "/Pictures"
   );

    addSidebarItem(
        "🎵  Music",
        QDir::homePath() + "/Music"
   );

    addSidebarItem(
        "🎬  Videos",
        QDir::homePath() + "/Videos"
   );

    m_fileList->setViewMode(QListView::ListMode);
    m_fileList->setIconSize(QSize(32, 32));
    m_fileList->setSpacing(4);

    // -----------------------------
    // Main layout
    // -----------------------------

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 10, 10, 10);
    layout->addLayout(navigationLayout);

    auto *contentLayout = new QHBoxLayout;

    contentLayout->addWidget(m_sidebar);
    contentLayout->addWidget(m_fileList);

    layout->addLayout(contentLayout);
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
    connect(
        m_sidebar,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item)
        {
            openSidebarLocation(item);
        }
    );
    connect(
        m_newButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::showNewMenu
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

    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QFileInfo info(path);

    if (info.isDir())
    {
        navigateTo(path);
        return;
    }

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

void PikaFileManager::openSidebarLocation(
    QListWidgetItem *item
)
{
    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QDir directory(path);

    if (!directory.exists())
        return;

    navigateTo(
        directory.absolutePath()
    );
}
void PikaFileManager::showNewMenu()
{
    QMenu menu(this);

    QAction *newFolder =
        menu.addAction("📁 New Folder");

    QAction *newFile =
        menu.addAction("📄 New Text File");

    QAction *selected =
        menu.exec(
            m_newButton->mapToGlobal(
                QPoint(
                    0,
                    m_newButton->height()
                )
            )
        );

    if (selected == newFolder)
    {
        createFolder();
    }
    else if (selected == newFile)
    {
        createTextFile();
    }
}

void PikaFileManager::createFolder()
{
    bool ok = false;

    QString name =
        QInputDialog::getText(
            this,
            "New Folder",
            "Folder name:",
            QLineEdit::Normal,
            "New Folder",
            &ok
        );

    if (!ok || name.trimmed().isEmpty())
        return;

    name = name.trimmed();

    QDir directory(m_currentPath);

    if (!directory.mkdir(name))
    {
        QMessageBox::warning(
            this,
            "Could not create folder",
            "The folder could not be created.\n"
            "It may already exist or you may not have permission."
        );

        return;
    }

    loadDirectory(m_currentPath);
}

void PikaFileManager::createTextFile()
{
    bool ok = false;

    QString name =
        QInputDialog::getText(
            this,
            "New Text File",
            "File name:",
            QLineEdit::Normal,
            "New Text File.txt",
            &ok
        );

    if (!ok || name.trimmed().isEmpty())
        return;

    name = name.trimmed();

    QString filePath =
        QDir(m_currentPath).filePath(name);

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
            this,
            "Could not create file",
            "The file could not be created.\n"
            "You may not have permission to write here."
        );

        return;
    }

    file.close();

    loadDirectory(m_currentPath);
}
