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

//快排
int Paritition(vector<int>& a, int l, int r) {
    int key = a[l];
    while (l < r)
    {
        while (l < r && a[r] >= key)
        {
            r--;
        }
        a[l] = a[r];
        while (l < r && a[l] <= key)
        {
            l++;
        }
        a[r] = a[l];
    }
    a[l] = key;
    return l;
}

void quickSort(vector<int>& a, int l, int r) {
    if (l < r) {
        int pivot = Paritition(a, l, r);
        quickSort(a, l, pivot - 1);
        quickSort(a, pivot + 1, r);
    }
}

//堆排
void max_heap(vector<int>& a, int l, int r) {
    int dad = l;
    int son = dad * 2 + 1;
    while (son <= r)
    {
        //比较两子节点，选最大
        if (son + 1 <= r && a[son] < a[son + 1]) son++;
        //父大于子，调整完毕，结束
        if (a[dad] > a[son]) return;
        else //交换父子节点，继续往下比较
        {
            swap(a[dad], a[son]);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

void heap_sort(vector<int>& a, int len) {
    //从最后一个父节点开始
    for (int i = len / 2 - 1; i >= 0; i--)
    {
        max_heap(a, i, len - 1);
    }
    //第一个元素和已排好的元素前一位交换，再重新调整，直到排序完毕
    for (int i = len - 1; i > 0; i--)
    {
        swap(a[0], a[i]);
        max_heap(a, 0, i - 1);
    }
}

int main() {
    /*cout << calSquare(10000) << endl;
    system("pause");
    system("cls");

    displayCircle();
    system("pause");
    system("cls");

    int cnt = 100000;
    pair<float, float> res = MontyHallParadox(cnt);
    cout << "keep win:" << res.first / cnt << "\tswitch win:" << res.second / cnt << endl;*/

    vector<int> a = { 3,7,5,2,1 };
    quickSort(a, 0, a.size() - 1);
    for (auto i : a) cout << i << endl;

    vector<int> b = { 3,7,5,2,1,4,6 };
    heap_sort(b, b.size());
    for (auto i : b) cout << i << endl;

    return 0;
}