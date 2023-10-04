#include <bits\stdc++.h>
#include "Map.h"

using namespace std;

int main() {
    int x, y;
    Map M(5);
    M.M_print();
    cout << "enter any key to start the game";
    system("pause");
    while (1)
    {
        system("cls");
        M.M_rebuild();
        M.M_print();
        cin >> x >> y;
        while (!(0 <= x <= M.get_Msize() && 0 <= y <= M.get_Msize())) {
            cout << "input wrong num, please re-input" << endl;
            cin >> x >> y;
        }
        if (M.M_judgeHit(x, y)) {
            cout << "you guess right" << endl;
        }
        else {
            cout << "you guess wrong" << endl;
        }
        M.M_tprint();
        if (M.M_judgeWon()) {
            cout << "you won the game" << endl;
            system("pause");
            break;
        }
        system("pause");
    }

    return 0;
}