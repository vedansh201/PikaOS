#pragma once

#include <QWidget>

class QLabel;
class PikachuWidget;   // <-- OUTSIDE the Desktop class

class Desktop : public QWidget
{


public:
    explicit Desktop(QWidget *parent = nullptr);

private:
    void setupUi();
    void setupBackground();
    void setupWallpaper();
    void setupPikachu();

    PikachuWidget *m_pikachu = nullptr;
};