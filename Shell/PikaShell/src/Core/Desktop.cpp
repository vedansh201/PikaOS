#include "Core/Desktop.hpp"
#include "Widgets/PikachuWidget.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "Core/PikaController.hpp"

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

    auto *controller = new PikaController(this);

    controller->setWidget(m_pikachu);

    m_pikachu->setController(controller);

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

