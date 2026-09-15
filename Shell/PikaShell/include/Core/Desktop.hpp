#pragma once

#include <QWidget>

class QLabel;
class PikachuWidget;
class PikaApplicationManager;

class Desktop : public QWidget
{
public:
    explicit Desktop(PikaApplicationManager *appManager,
                     QWidget *parent = nullptr);

private:
    void setupUi();
    void setupBackground();
    void setupWallpaper();
    void setupPikachu();

    PikachuWidget *m_pikachu = nullptr;
    PikaApplicationManager *m_appManager = nullptr;
};