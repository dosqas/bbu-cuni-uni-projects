#include "gui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GUI window;
    window.question();

    return app.exec();
}
