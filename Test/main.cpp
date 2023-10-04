#include <bits\stdc++.h>

using namespace std;

//int minOperations(vector<int>& nums) {
//    if (nums.size() == 2 && nums[0] == nums[1]) return 1;
//    sort(nums.begin(), nums.end());
//    int res = 0;
//    int pre = 0;
//    for (int i = 0; i < nums.size(); ++i) {
//        while (i < nums.size() - 1 && nums[pre] == nums[i + 1]) {
//            ++i;
//        }
//        cout << pre << i;
//        if (i - pre + 1 >= 2) {
//            if ((i - pre + 1) % 3 == 1) {
//                cout << " 1 ";
//                res += (i - pre + 1) / 3 > 0 ? ceil((i - pre + 1) / 3) - 1 : (i - pre + 1) / 3;
//                res += 2;
//            }
//            else if ((i - pre + 1) % 3 == 2) {
//                cout << " 2 ";
//                res += (i - pre + 1) / 3 > 0 ? ceil((i - pre + 1) / 3) : (i - pre + 1) / 3;
//                res += 1;
//            }
//            else {
//                cout << " 3 ";
//                res += (i - pre + 1) / 3;
//            }
//        }
//        else { return -1; }
//        pre = i + 1;
//    }
//    return res;
//}
//
//int maxProfit(vector<int>& prices) {
//    int mn = 0, mx = 0;
//    int res = 0;
//    for (int i = 0; i < prices.size(); i++) {
//        if (prices[i] < prices[mn]) {
//            mn = i;
//        }
//        else if (prices[i] > prices[mx]) {
//            mx = i;
//        }
//        if (mx > mn)
//            res = max(res, prices[mx] - prices[mn]);
//    }
//    return res;
//}

int calSquare(int  x) {
    int last = x / 2, res = x;
    if (x > 0) {
        while (res - last)
        {
            last = res;
            res = (res + x / res) / 2;
        }
    }
    return res;
}

void displayCircle() {
    vector<vector<char>> display(100, vector<char>(100, '#'));
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (pow(i - 50, 2) + pow(j - 50, 2) <= 25) display[i][j] = 'O';
        }
    }
    for (auto di : display)
    {
        for (auto d : di)
        {
            cout << d;
        }
        cout << endl;
    }
}

pair<float, float> MontyHallParadox(const int cnt, const int doors = 3) {
    srand(unsigned int(time(0)));
    float cntStickWin = 0, cntSwitchWin = 0;

    auto StickWin = [](const int winDoor, const int choice, float& cntStickWin) {
        if (choice == winDoor) {
            cntStickWin++;
        }
    };

    auto SwitchWin = [](const int _doors, const int winDoor, const int choice, float& cntSwitchWin) {
        int openDoor = rand() % _doors;
        while (openDoor == choice || openDoor == winDoor)
        {
            openDoor = rand() % _doors;
        }
        int newChoice = rand() % _doors;
        while (newChoice == choice || newChoice == openDoor)
        {
            newChoice = rand() % _doors;
        }
        if (newChoice == winDoor) cntSwitchWin++;
    };

    for (int i = cnt; i >= 0; --i)
    {
        int winDoor = rand() % doors;
        int choice = rand() % doors;
        StickWin(winDoor, choice, cntStickWin);
        SwitchWin(doors, winDoor, choice, cntSwitchWin);
    }

    return make_pair(cntStickWin, cntSwitchWin);
}

int main() {
    cout << calSquare(10000) << endl;
    system("pause");
    system("cls");

    displayCircle();
    system("pause");
    system("cls");

    int cnt = 100000;
    pair<float, float> res = MontyHallParadox(cnt);
    cout << "keep win:" << res.first / cnt << "\tswitch win:" << res.second / cnt << endl;

    return 0;
}