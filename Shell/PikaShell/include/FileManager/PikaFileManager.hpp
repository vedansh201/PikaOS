#pragma once

#include <QWidget>
#include <QString>
#include <QStringList>

class QListWidget;
class QLineEdit;
class QPushButton;

class PikaFileManager : public QWidget
{
    Q_OBJECT

public:
    explicit PikaFileManager(QWidget *parent = nullptr);

private:
    QListWidget *m_fileList;
    QLineEdit *m_pathBar;

    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
    QPushButton *m_upButton;

    QString m_currentPath;

    QStringList m_backHistory;
    QStringList m_forwardHistory;

    void loadDirectory(const QString &path);

    void openItem();

    void goBack();
    void goForward();
    void goUp();

    void navigateTo(const QString &path);
};