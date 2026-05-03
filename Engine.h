#ifndef ENGINE_H
#define ENGINE_H
#include <map>
#include <string>
#include <vector>
#include "User.h"
#include "Rule.h"
#include "Storage.h"

class Engine {
private:
    std::map<std::string, User*> users;
    Storage<Rule*> ruleStorage;

    std::map<std::string, std::string> credentials;
    std::map<std::string, std::string> recoveryPins;
    std::string currentUser;

    Engine();

public:
    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;

    static Engine& getInstance();

    void registerUser(std::string name, std::string pass, std::string pin);
    void loginUser(std::string name, std::string pass);
    void logout();
    void resetPassword(std::string name, std::string pin, std::string newPass);
    void changePassword(std::string oldPass, std::string newPass);
    bool isLoggedIn() const;
    bool hasUsers() const;


    bool userExists(std::string name) const;
    bool isVIP(std::string name) const;
    int getInterestCount(std::string name) const;
    std::vector<std::string> getUserInterests(std::string name) const;
    int getTotalUsers() const;


    void addUser(std::string name);
    void addVIP(std::string name);
    void addInterest(std::string name, std::string interest);

    void addRule(std::string c, std::string r);
    int findRuleIndex(std::string cond) const;
    void deleteRule(int index);
    void editRule(int index, std::string newCond, std::string newRec);
    void showRules() const;

    void recommend(std::string name);
    void showAllUsers() const;

    void deleteUser(std::string name);
    void deleteInterest(std::string name, int index);


    void searchByName(std::string keyword) const;
    void filterByInterest(std::string interest) const;
    void matchUsers(std::string name) const;

    void showStats() const;

    void importUsers(std::string filepath, bool asVIP);
    void autoSave();

    void callFriendDemo(std::string name);
    void callOperatorDemo(std::string name);
    void callEqualityDemo(std::string name);

    ~Engine();
};

#endif
