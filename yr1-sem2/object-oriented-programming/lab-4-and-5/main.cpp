#include "ui.h"
#include "tests.h"


int main()
{
    /*cout << "© dosqasoft 2024\n";
    cout << "BOOTING UP. ";
    Sleep(750);
    cout << ". ";
    Sleep(750);
    cout << ".";
    Sleep(1250);
    UI ui = UI();

    while (ui.main_menu() != 0) {}*/

    Tests tests = Tests();
    tests.test_all();
    return 0;
}