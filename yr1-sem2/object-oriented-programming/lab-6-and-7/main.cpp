#include "ui.h"

int main()
{
    {
        cout << "© dosqasoft 2024\n";
        cout << "BOOTING UP. ";
        Sleep(750);
        cout << ". ";
        Sleep(750);
        cout << ".";
        Sleep(1250);
        UI ui = UI();

        ui.question();
        while (ui.main_menu() != 0) {}
    }

    _CrtDumpMemoryLeaks();
    return 0;
}