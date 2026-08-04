#include "Core/Desktop.hpp"
#include "Widgets/PikachuWidget.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

Desktop::Desktop(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void Desktop::setupUi()
{
    setupBackground();
    setupWallpaper();
    setupPikachu();
}

void Desktop::setupPikachu()
{
    qDebug() << "setupPikachu called";

    m_pikachu = new PikachuWidget(this);

    m_pikachu->move(850, 400);
    m_pikachu->show();
}

void Desktop::setupBackground()
{
    setStyleSheet(R"(
        background-color: #1E1F29;
    )");
}

void Desktop::setupWallpaper()
{
    // Wallpaper support will come later.
}

