#include <QApplication>
#include "Core/PikaWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PikaWindow window;
    window.show();

    return app.exec();
}