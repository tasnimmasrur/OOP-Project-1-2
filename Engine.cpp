#include "Engine.h"
#include "Exception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <windows.h>

static void SetEngineColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

Engine::Engine() {
    std::ifstream fAuth("auth_data.txt");
    if (fAuth) {
        std::string n, p, pin;
        while (fAuth >> n >> p >> pin) { credentials[n] = p; recoveryPins[n] = pin; }
        fAuth.close();
    }

    std::ifstream fRules("rules_data.txt");
    if (fRules) {
        std::string c, r;
        while (fRules >> c >> r) { ruleStorage.add(RuleFactory::createRule(c, r)); }
        fRules.close();
    }

    std::ifstream fNormal("normal_users.txt");
    if (fNormal) {
        std::string line;
        while (std::getline(fNormal, line)) {
            std::stringstream ss(line);
            std::string name, interest;
            ss >> name;
            if (users.find(name) == users.end()) { users[name] = new User(name); }
            while (ss >> interest) users[name]->addInterest(interest);
        }
        fNormal.close();
    }

    std::ifstream fVIP("vip_users.txt");
    if (fVIP) {
        std::string line;
        while (std::getline(fVIP, line)) {
            std::stringstream ss(line);
            std::string name, interest;
            ss >> name;
            if (users.find(name) == users.end()) { users[name] = new VIPUser(name); }
            while (ss >> interest) users[name]->addInterest(interest);
        }
        fVIP.close();
    }
}

Engine& Engine::getInstance() {
    static Engine instance;
    return instance;
}

bool Engine::hasUsers() const { return !credentials.empty(); }
bool Engine::userExists(std::string name) const { return users.find(name) != users.end(); }

bool Engine::isVIP(std::string name) const {
    if (!userExists(name)) return false;
    return dynamic_cast<VIPUser*>(users.at(name)) != nullptr;
}

int Engine::getInterestCount(std::string name) const {
    if (!userExists(name)) return 0;
    return users.at(name)->getInterests().size();
}

std::vector<std::string> Engine::getUserInterests(std::string name) const {
    if (!userExists(name)) return {};
    return users.at(name)->getInterests();
}

int Engine::getTotalUsers() const {
    return users.size();
}

void Engine::showRules() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    📋 ALL RECOMMENDATION RULES                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n\n";

    auto rules = ruleStorage.getAll();
    if (rules.empty()) {
        std::cout << "   No recommendation rules added yet.\n";
        return;
    }

    std::cout << "   No.   Condition" << std::setw(20) << " → Recommendation\n";
    std::cout << "   -----------------------------------------------------------------\n";

    int count = 1;
    for (auto r : rules) {
        std::cout << "   " << std::setw(2) << count++ << ".   "
                  << std::left << std::setw(15) << r->getCond()
                  << " → " << r->getRec() << "\n";
    }
    std::cout << "\n   Total Rules: " << rules.size() << "\n\n";
}

int Engine::findRuleIndex(std::string cond) const {
    auto rules = ruleStorage.getAll();
    for (size_t i = 0; i < rules.size(); i++) {
        if (rules[i]->getCond() == cond) return i;
    }
    return -1;
}

void Engine::addRule(std::string c, std::string r) {
    if (!isLoggedIn()) throw MyException("Access Denied!");

    if (findRuleIndex(c) != -1) {
        throw MyException("Rule for '" + c + "' already exists! Please use [3] edit_rule.");
    }

    ruleStorage.add(RuleFactory::createRule(c, r));
    autoSave();
    SetEngineColor(10);
    std::cout << "[SUCCESS] Rule added: " << c << " → " << r << "\n";
    SetEngineColor(7);
}

void Engine::deleteRule(int index) {
    if (!isLoggedIn()) throw MyException("Access Denied!");

    auto rules = ruleStorage.getAll();
    if (index < 1 || index > (int)rules.size()) {
        throw MyException("Invalid rule index! Check the rule list first.");
    }

    delete rules[index - 1];
    ruleStorage.remove(index - 1);

    autoSave();
    SetEngineColor(10);
    std::cout << "[SUCCESS] Rule deleted successfully.\n";
    SetEngineColor(7);
}

void Engine::editRule(int index, std::string newCond, std::string newRec) {
    if (!isLoggedIn()) throw MyException("Access Denied!");

    auto rules = ruleStorage.getAll();
    if (index < 1 || index > (int)rules.size()) {
        throw MyException("Invalid rule index! Check the rule list first.");
    }

    int existingIndex = findRuleIndex(newCond);
    if (existingIndex != -1 && existingIndex != (index - 1)) {
        throw MyException("Conflict! Rule for '" + newCond + "' already exists elsewhere!");
    }

    delete rules[index - 1];
    ruleStorage.set(index - 1, RuleFactory::createRule(newCond, newRec));

    autoSave();
    SetEngineColor(10);
    std::cout << "[SUCCESS] Rule updated to: " << newCond << " → " << newRec << "\n";
    SetEngineColor(7);
}


void Engine::registerUser(std::string name, std::string pass, std::string pin) {
    if (credentials.find(name) != credentials.end()) throw MyException("Username already taken!");
    credentials[name] = pass;
    recoveryPins[name] = pin;
    autoSave();
    std::cout << "[SUCCESS] Registered successfully! Recovery PIN: " << pin << "\n";
}

void Engine::loginUser(std::string name, std::string pass) {
    if (credentials.find(name) == credentials.end()) throw MyException("User not found!");
    if (credentials[name] != pass) throw MyException("Wrong password!");
    currentUser = name;
    std::cout << "[SUCCESS] Login successful! Welcome, " << currentUser << ".\n";
}

void Engine::resetPassword(std::string name, std::string pin, std::string newPass) {
    if (credentials.find(name) == credentials.end()) throw MyException("User not found!");
    if (recoveryPins[name] != pin) throw MyException("Access Denied: Invalid PIN!");
    credentials[name] = newPass;
    autoSave();
    std::cout << "[SUCCESS] Password reset successful!\n";
}

void Engine::changePassword(std::string oldPass, std::string newPass) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (credentials[currentUser] != oldPass) throw MyException("Incorrect old password!");
    credentials[currentUser] = newPass;
    autoSave();
    std::cout << "[SUCCESS] Password changed!\n";
}

void Engine::logout() {
    currentUser = "";
    std::cout << "[INFO] Logged out.\n";
}

bool Engine::isLoggedIn() const { return !currentUser.empty(); }

void Engine::addUser(std::string name) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (users.find(name) != users.end()) throw MyException("User exists!");
    users[name] = new User(name);
    autoSave();
    std::cout << "[SUCCESS] Normal User added.\n";
}

void Engine::addVIP(std::string name) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (users.find(name) != users.end()) throw MyException("User exists!");
    users[name] = new VIPUser(name);
    autoSave();
    std::cout << "[SUCCESS] VIP User added.\n";
}

void Engine::addInterest(std::string name, std::string interest) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (!userExists(name)) throw MyException("User not found!");
    users[name]->addInterest(interest);
    autoSave();
}


void Engine::recommend(std::string name) {
    if (!userExists(name)) throw MyException("User not found!");
    std::cout << "\n[RECOMMENDATIONS] for " << name << ":\n";
    bool found = false;
    for (const auto& i : users[name]->getInterests()) {
        for (auto r : ruleStorage.getAll()) {
            std::string res = r->apply(i);
            if (!res.empty()) {
                std::cout << "  -> " << res << " (Because of: " << i << ")\n";
                found = true;
            }
        }
    }
    if (!found) std::cout << "  -> No recommendations available yet.\n";
}

void Engine::showAllUsers() const {
    if (users.empty()) {
        SetEngineColor(14);
        std::cout << "[INFO] No users in database.\n";
        SetEngineColor(7);
        return;
    }

    std::vector<std::pair<std::string, User*>> userList(users.begin(), users.end());
    int total = userList.size();
    int perPage = 5;
    int totalPages = (total + perPage - 1) / perPage;
    int currPage = 1;

    while (true) {
        system("cls");
        SetEngineColor(11);
        std::cout << "================================================================================\n";
        std::cout << "                           📋 ALL REGISTERED USERS\n";
        std::cout << "================================================================================\n";
        SetEngineColor(14);
        std::cout << " Page " << currPage << " of " << totalPages
                  << "                                              (Showing "
                  << ((currPage - 1) * perPage + 1) << "-" << std::min(currPage * perPage, total) << " of " << total << ")\n";
        SetEngineColor(11);
        std::cout << "--------------------------------------------------------------------------------\n";
        SetEngineColor(7);

        int start = (currPage - 1) * perPage;
        int end = std::min(start + perPage, total);

        for (int i = start; i < end; i++) {
            userList[i].second->show();
            auto ints = userList[i].second->getInterests();
            if (ints.empty()) {
                std::cout << "   Interests: None\n";
            } else {
                std::cout << "   Interests: ";
                for (size_t k = 0; k < ints.size(); ++k) {
                    std::cout << ints[k] << (k == ints.size() - 1 ? "" : ", ");
                }
                std::cout << "\n";
            }
            SetEngineColor(8);
            std::cout << "--------------------------------------------------------------------------------\n";
            SetEngineColor(7);
        }

        SetEngineColor(14);
        std::cout << "\n [N] Next Page    [P] Previous Page    [B] Back to Menu\n\n";
        SetEngineColor(7);
        std::cout << "[CMD] >> ";

        std::string choice;
        std::cin >> choice;

        if (choice == "N" || choice == "n") {
            if (currPage < totalPages) currPage++;
        } else if (choice == "P" || choice == "p") {
            if (currPage > 1) currPage--;
        } else if (choice == "B" || choice == "b") {
            break;
        }
    }
}

void Engine::searchByName(std::string keyword) const {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    std::string kw = keyword;
    for (auto& c : kw) c = tolower(c);
    bool found = false;
    std::cout << "\n[SEARCH] Results for '" << keyword << "':\n";
    std::cout << "==============================\n";
    for (const auto& pair : users) {
        std::string uname = pair.first;
        std::string lower = uname;
        for (auto& c : lower) c = tolower(c);
        if (lower.find(kw) != std::string::npos) {
            pair.second->show();
            auto ints = pair.second->getInterests();
            if (ints.empty()) {
                std::cout << "   Interests: None\n";
            } else {
                for (size_t i = 0; i < ints.size(); i++)
                    std::cout << "   " << (i+1) << ". " << ints[i] << "\n";
            }
            std::cout << "------------------------------\n";
            found = true;
        }
    }
    if (!found) std::cout << "[INFO] No user found matching '" << keyword << "'.\n";
}

void Engine::filterByInterest(std::string interest) const {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    std::string kw = interest;
    for (auto& c : kw) c = tolower(c);
    bool found = false;
    std::cout << "\n[FILTER] Users with interest matching '" << interest << "':\n";
    std::cout << "==============================\n";
    for (const auto& pair : users) {
        auto ints = pair.second->getInterests();
        for (const auto& i : ints) {
            std::string lower = i;
            for (auto& c : lower) c = tolower(c);
            if (lower.find(kw) != std::string::npos) {
                pair.second->show();
                for (size_t k = 0; k < ints.size(); k++)
                    std::cout << "   " << (k+1) << ". " << ints[k] << "\n";
                std::cout << "------------------------------\n";
                found = true;
                break;
            }
        }
    }
    if (!found) std::cout << "[INFO] No user found with interest matching '" << interest << "'.\n";
}

void Engine::matchUsers(std::string name) const {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (!userExists(name)) throw MyException("User '" + name + "' not found!");
    auto myInts = users.at(name)->getInterests();
    if (myInts.empty()) {
        std::cout << "[INFO] User has no interests to match with others.\n";
        return;
    }
    std::cout << "\n[MATCH] Users with similar interests as '" << name << "':\n";
    std::cout << "==========================================\n";
    bool found = false;
    for (const auto& pair : users) {
        if (pair.first == name) continue;
        int score = 0;
        for (const auto& i : pair.second->getInterests()) {
            std::string a = i; for(auto& c: a) c = tolower(c);
            for (const auto& mi : myInts) {
                std::string b = mi; for(auto& c: b) c = tolower(c);
                if (a == b) { score++; break; }
            }
        }
        if (score > 0) {
            std::cout << "  -> " << pair.first << " (Match Score: " << score << " shared interests)\n";
            found = true;
        }
    }
    if (!found) std::cout << "  -> No matches found.\n";
    std::cout << "==========================================\n";
}

void Engine::deleteUser(std::string name) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (!userExists(name)) throw MyException("User '" + name + "' not found!");
    delete users[name];
    users.erase(name);
    remove((name + "_normal.txt").c_str());
    remove((name + "_vip.txt").c_str());
    autoSave();
    std::cout << "[SUCCESS] User '" << name << "' permanently deleted.\n";
}

void Engine::deleteInterest(std::string name, int index) {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    if (!userExists(name)) throw MyException("User '" + name + "' not found!");
    auto interests = users[name]->getInterests();
    if (index < 1 || index > (int)interests.size())
        throw MyException("Invalid index! Enter a number between 1 and " + std::to_string(interests.size()));
    std::string uname = users[name]->getName();
    bool vip = isVIP(uname);
    interests.erase(interests.begin() + index - 1);
    delete users[name];
    users[uname] = vip ? new VIPUser(uname) : new User(uname);
    for (const auto& i : interests) {
        users[uname]->addInterest(i);
    }
    autoSave();
    std::cout << "[SUCCESS] Interest deleted successfully.\n";
}

void Engine::importUsers(std::string filepath, bool asVIP) {
    if (!isLoggedIn()) throw MyException("Access Denied: Please login first!");
    std::ifstream f(filepath);
    if (!f) throw MyException("File not found! Please check the file path and try again.");
    int count = 0;
    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string name, interest;
        ss >> name;
        if (name.empty()) continue;
        if (users.find(name) == users.end()) {
            if (asVIP) users[name] = new VIPUser(name);
            else users[name] = new User(name);
            count++;
        }
        while (ss >> interest) {
            try {
                users[name]->addInterest(interest);
            } catch (...) { }
        }
    }
    f.close();
    autoSave();
    std::cout << "[SUCCESS] " << count << " users successfully imported from " << filepath << "!\n";
}

void Engine::autoSave() {
    std::ofstream fNormal("normal_users.txt");
    std::ofstream fVIP("vip_users.txt");
    std::ofstream fAll("all_users_combined.txt");

    std::map<std::string, std::vector<std::string>> interestGroups;

    for (const auto& u : users) {
        std::string uname = u.first;
        auto ints = u.second->getInterests();
        bool isVip = (dynamic_cast<VIPUser*>(u.second) != nullptr);

        fAll << uname << " [" << (isVip ? "VIP" : "Normal") << "] Interests: ";
        if(ints.empty()) fAll << "None";
        for(const auto& i : ints) fAll << i << " ";
        fAll << "\n";

        for(const auto& i : ints) {
            std::string lowerInt = i;
            for(auto& c : lowerInt) c = tolower(c);
            interestGroups[lowerInt].push_back(uname);
        }

        if (isVip) {
            fVIP << uname;
            for (const auto& i : ints) fVIP << " " << i;
            fVIP << "\n";

            std::ofstream fInd(uname + "_vip.txt");
            fInd << uname;
            for (const auto& i : ints) fInd << " " << i;
            fInd << "\n";
            fInd.close();
        } else {
            fNormal << uname;
            for (const auto& i : ints) fNormal << " " << i;
            fNormal << "\n";

            std::ofstream fInd(uname + "_normal.txt");
            fInd << uname;
            for (const auto& i : ints) fInd << " " << i;
            fInd << "\n";
            fInd.close();
        }
    }
    fNormal.close(); fVIP.close(); fAll.close();

    for (const auto& group : interestGroups) {
        std::ofstream fInt("interest_" + group.first + ".txt");
        fInt << "=== Users interested in: " << group.first << " ===\n";
        for (const auto& name : group.second) {
            fInt << "- " << name << "\n";
        }
        fInt.close();
    }

    std::ofstream fRules("rules_data.txt");
    for (auto r : ruleStorage.getAll()) {
        fRules << r->getCond() << " " << r->getRec() << "\n";
    }
    fRules.close();

    std::ofstream fAuth("auth_data.txt");
    for (const auto& c : credentials) {
        fAuth << c.first << " " << c.second << " " << recoveryPins[c.first] << "\n";
    }
    fAuth.close();
}

void Engine::showStats() const {
    if (!isLoggedIn()) throw MyException("Access Denied!");
    int totalUsers = 0, vipCount = 0, normalCount = 0;
    int totalInterests = 0, maxInterests = 0;
    std::string mostActiveUser = "";
    std::map<std::string, int> interestFreq;

    for (const auto& pair : users) {
        totalUsers++;
        if (isVIP(pair.first)) vipCount++;
        else normalCount++;

        auto ints = pair.second->getInterests();
        totalInterests += ints.size();

        if ((int)ints.size() > maxInterests) {
            maxInterests = ints.size();
            mostActiveUser = pair.first;
        }

        for (const auto& i : ints) {
            std::string lower = i;
            for (auto& c : lower) c = tolower(c);
            interestFreq[lower]++;
        }
    }

    std::string topInterest = "None";
    int topCount = 0;
    for (const auto& pair : interestFreq) {
        if (pair.second > topCount) {
            topCount = pair.second;
            topInterest = pair.first;
        }
    }

    double avg = (totalUsers > 0) ? (double)totalInterests / totalUsers : 0.0;

    std::cout << "\n[STATS] ====== SYSTEM ANALYTICS ======\n";
    std::cout << "--------------------------------------\n";
    std::cout << "  Total Users       : " << totalUsers << "\n";
    std::cout << "  Normal Users      : " << normalCount << "\n";
    std::cout << "  VIP Users         : " << vipCount << "\n";
    std::cout << "  Total Interests   : " << totalInterests << "\n";
    std::cout << "  Avg Interests/User: " << avg << "\n";
    std::cout << "--------------------------------------\n";

    if (!mostActiveUser.empty())
        std::cout << "  Most Active User  : " << mostActiveUser << " (" << maxInterests << " interests)\n";
    if (topInterest != "None")
        std::cout << "  Top Interest      : " << topInterest << " (" << topCount << " users)\n";

    std::cout << "--------------------------------------\n";

    if (!interestFreq.empty()) {
        std::cout << "  Interest Ranking:\n";
        std::vector<std::pair<std::string, int>> sorted(interestFreq.begin(), interestFreq.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const std::pair<std::string,int>& a, const std::pair<std::string,int>& b) {
                return b.second < a.second;
            }
        );

        int limit = std::min((int)sorted.size(), 3);
        for (int i = 0; i < limit; i++) {
            std::cout << "    " << (i+1) << ". " << sorted[i].first << " — " << sorted[i].second << " user(s)\n";
        }
    }
    std::cout << "  Registered Admins : " << credentials.size() << "\n";
    std::cout << "======================================\n";
}


void Engine::callFriendDemo(std::string name) {
    if (!userExists(name)) throw MyException("User not found!");
    showUserPrivate(*users[name]);
}

void Engine::callOperatorDemo(std::string name) {
    if (!userExists(name)) throw MyException("User not found!");
    std::cout << *users[name] << std::endl;
}


void Engine::callEqualityDemo(std::string name) {
    if (!userExists(name)) throw MyException("User not found!");

    User dummy(name);
    User dummy2("someone_else");
    if (*users[name] == dummy)
        std::cout << "  -> operator==: '" << name << "' == '" << dummy.getName() << "' → TRUE\n";
    if (!(*users[name] == dummy2))
        std::cout << "  -> operator==: '" << name << "' == '" << dummy2.getName() << "' → FALSE\n";
}

Engine::~Engine() {
    for (auto u : users) delete u.second;
    for (auto r : ruleStorage.getAll()) delete r;
}
