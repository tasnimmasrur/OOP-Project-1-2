#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>
#include "Engine.h"
#include "Exception.h"
using namespace std;

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ClearScreen() { system("cls"); }

void SleepMS(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

void showProgressBar(const string& msg) {
    SetColor(11);
    cout << "\n   " << msg << "\n   [";
    SetColor(10);
    for (int i = 0; i <= 30; i++) { cout << "█"; SleepMS(40); }
    SetColor(7);
    cout << "] Done!\n\n";
}

void showStatusBar(Engine& e) {
    SetColor(8);
    cout << "══════════════════════════════════════════════════════════════════════════════\n";
    SetColor(7);
    cout << " 👤 Logged in as: tasnim";
    cout << "   |   Total Users: " << e.getTotalUsers();
    cout << "   |   Time: ";
    SetColor(14); cout << __TIME__; SetColor(7);
    cout << "\n══════════════════════════════════════════════════════════════════════════════\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Engine& e = Engine::getInstance();
    string cmd;
    int currentMenu = 0;

    while (true) {
        try {
            if (!e.isLoggedIn()) {
                currentMenu = 0;
                ClearScreen();
                SetColor(11);
                cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
                cout << "║                     🔐 SECURE LOGIN SYSTEM                                   ║\n";
                cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n\n";
                SetColor(7);
                cout << "   [1] login\n   [2] forgot_pass\n   [0] exit\n\n";
                SetColor(14); cout << "[AUTH CMD] >> "; SetColor(7);
                cin >> cmd;

                if (!e.hasUsers() && (cmd == "1" || cmd == "register")) {
                    string n, p, pin;
                    cout << "\nUsername: "; cin >> n;
                    cout << "Password: "; cin >> p;
                    cout << "PIN: "; cin >> pin;
                    SetColor(10); e.registerUser(n, p, pin); SetColor(7);
                } else if (cmd == "1" || cmd == "login") {
                    string n, p;
                    cout << "\nUsername: "; cin >> n;
                    cout << "Password: "; cin >> p;
                    SetColor(10); e.loginUser(n, p); SetColor(7);
                } else if (cmd == "2" || cmd == "forgot_pass") {
                    string n, pin, np;
                    cout << "\nUsername: "; cin >> n;
                    cout << "PIN: "; cin >> pin;
                    cout << "New Pass: "; cin >> np;
                    SetColor(10); e.resetPassword(n, pin, np); SetColor(7);
                } else if (cmd == "0" || cmd == "exit") break;
            } else {
                ClearScreen();
                SetColor(13);
                cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
                cout << "║  🚀  MASTER ADMIN PANEL  -  ADVANCED USER MANAGEMENT SYSTEM                  ║\n";
                cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n\n";
                SetColor(14); cout << "   👤 Logged in as : tasnim\n"; SetColor(7);

                if (currentMenu == 0) {
                    cout << "\n   [1] 👥 User Management        (Add, View, Delete, Search)\n";
                    cout << "   [2] 🎯 Recommendation Engine  (Add/Edit Rules, Recommend)\n";
                    cout << "   [3] ⚙️  System Tools          (Stats, Import, Change Pass)\n\n";
                    cout << "   [8] 🚪 Logout\n";
                    cout << "   [0] ❌ Exit\n\n";
                    showStatusBar(e);
                    SetColor(14); cout << "[MAIN] >> "; SetColor(7);
                    cin >> cmd;

                    if (cmd == "1") currentMenu = 1;
                    else if (cmd == "2") currentMenu = 2;
                    else if (cmd == "3") currentMenu = 3;
                    else if (cmd == "8") { e.logout(); currentMenu = 0; continue; }
                    else if (cmd == "0" || cmd == "exit") break;
                }
                else if (currentMenu == 1) {
                    SetColor(11); cout << "\n--- 👥 USER MANAGEMENT ---\n"; SetColor(7);
                    cout << "   [1] Create Normal User\n   [2] Create VIP User\n";
                    cout << "   [3] Show All Users (Paged)\n   [4] Search by Name\n";
                    cout << "   [5] Filter by Interest\n   [6] Match Users\n";
                    cout << "   [7] Add Interest\n   [8] Delete Interest\n";
                    cout << "   [9] Delete User\n   [B] Back to Main Menu\n\n";
                    showStatusBar(e);
                    SetColor(14); cout << "[USER] >> "; SetColor(7);
                    cin >> cmd;

                    if (cmd == "B" || cmd == "b") { currentMenu = 0; continue; }
                    else if (cmd == "1") { string n; cout << "\nName: "; cin >> n; SetColor(10); e.addUser(n); SetColor(7); }
                    else if (cmd == "2") { string n; cout << "\nName: "; cin >> n; SetColor(10); e.addVIP(n); SetColor(7); }
                    else if (cmd == "3") { e.showAllUsers(); continue; }
                    else if (cmd == "4") { string n; cout << "\nSearch: "; cin >> n; e.searchByName(n); }
                    else if (cmd == "5") { string n; cout << "\nInterest: "; cin >> n; e.filterByInterest(n); }
                    else if (cmd == "6") { string n; cout << "\nUsername: "; cin >> n; e.matchUsers(n); }
                    else if (cmd == "7") {
                        string n, i; cout << "\nUsername: "; cin >> n;
                        cout << "Interest to add: "; cin >> i;
                        SetColor(10); e.addInterest(n, i); SetColor(7);
                    }
                    else if (cmd == "8") {
                        string n; int idx; cout << "\nUsername: "; cin >> n;
                        e.searchByName(n);
                        cout << "\nEnter Interest Index to Delete: "; cin >> idx;
                        SetColor(10); e.deleteInterest(n, idx); SetColor(7);
                    }
                    else if (cmd == "9") { string n; cout << "\nUsername to delete: "; cin >> n; SetColor(10); e.deleteUser(n); SetColor(7); }
                }
                else if (currentMenu == 2) {
                    SetColor(11); cout << "\n--- 🎯 RECOMMENDATION ENGINE ---\n"; SetColor(7);
                    cout << "   [1] Add Rule\n   [2] View All Rules\n";
                    cout << "   [3] Edit Rule\n   [4] Delete Rule\n";
                    cout << "   [5] Get Recommendations for User\n   [B] Back to Main Menu\n\n";
                    showStatusBar(e);
                    SetColor(14); cout << "[RULES] >> "; SetColor(7);
                    cin >> cmd;

                    if (cmd == "B" || cmd == "b") { currentMenu = 0; continue; }
                    else if (cmd == "1") {
                        string c, r; cout << "\nCondition: "; cin >> c; cout << "Recommendation: "; cin >> r;
                        e.addRule(c, r);
                    }
                    else if (cmd == "2") { e.showRules(); }
                    else if (cmd == "3") {
                        e.showRules(); int idx; cout << "\nRule No. to edit: "; cin >> idx;
                        string c, r; cout << "New Condition: "; cin >> c; cout << "New Rec: "; cin >> r;
                        e.editRule(idx, c, r);
                    }
                    else if (cmd == "4") {
                        e.showRules(); int idx; cout << "\nRule No. to delete: "; cin >> idx;
                        e.deleteRule(idx);
                    }
                    else if (cmd == "5") { string n; cout << "\nUsername: "; cin >> n; e.recommend(n); }
                }
                else if (currentMenu == 3) {
                    SetColor(11); cout << "\n--- ⚙️ SYSTEM TOOLS ---\n"; SetColor(7);
                    cout << "   [1] System Statistics\n   [2] Import Users from File\n";
                    cout << "   [3] Change Password\n   [4] OOP Demo (Friend, Operator, This)\n   [B] Back to Main Menu\n\n";
                    showStatusBar(e);
                    SetColor(14); cout << "[SYS] >> "; SetColor(7);
                    cin >> cmd;

                    if (cmd == "B" || cmd == "b") { currentMenu = 0; continue; }
                    else if (cmd == "1") { showProgressBar("Generating System Statistics..."); e.showStats(); }
                    else if (cmd == "2") {

                        SetColor(11);
                        cout << "\n   [📝 FILE FORMAT HINT]\n";
                        cout << "   Create a .txt file. Inside it, write username followed by interests:\n";
                        SetColor(8);
                        cout << "   ----------------------------------------\n";
                        cout << "   tasnim coding ai\n";
                        cout << "   ratul hardware gaming\n";
                        cout << "   ----------------------------------------\n";
                        SetColor(7);

                        string file; bool asVip;
                        cout << "\nEnter Filename (e.g. data.txt or C:\ Users\ masru\ Desktop\ users.txt): "; cin >> file;
                        cout << "Import as VIP? (1 for Yes, 0 for No): "; cin >> asVip;
                        SetColor(10); e.importUsers(file, asVip); SetColor(7);
                    }
                    else if (cmd == "3") {
                        string oldp, newp; cout << "\nOld Password: "; cin >> oldp; cout << "New Password: "; cin >> newp;
                        SetColor(10); e.changePassword(oldp, newp); SetColor(7);
                    }
                    else if (cmd == "4") {

                        string n; cout << "\nEnter username to demo: "; cin >> n;
                        if (!e.userExists(n)) throw MyException("User not found!");

                        SetColor(11);
                        cout << "\n========== OOP DEMO ==========\n";
                        SetColor(7);


                        SetColor(14); cout << "\n[1] Friend Function:\n"; SetColor(7);
                        e.callFriendDemo(n);


                        SetColor(14); cout << "\n[2] operator<< (IO Overload):\n"; SetColor(7);
                        e.callOperatorDemo(n);


                        SetColor(14); cout << "\n[3] operator== (Equality):\n"; SetColor(7);
                        e.callEqualityDemo(n);

                        SetColor(10);
                        cout << "\n[INFO] this pointer used in User constructor (this->name, this->interests)\n";
                        SetColor(7);
                        cout << "==============================\n";
                    }
                }
            }
        } catch (const MyException& ex) {
            SetColor(12);
            cout << "\n[ERROR] " << ex.msg << endl;
            SetColor(7);
        }

        if(cmd != "3" || currentMenu != 1) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    return 0;
}
