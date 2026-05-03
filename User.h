#ifndef USER_H
#define USER_H
#include <iostream>
#include <vector>
#include <string>
#include "Logger.h"

class User {
protected:
    std::string name;
    std::vector<std::string>* interests;

public:
    static int totalUsers;

    User(std::string n = "Unknown");
    User(const User& u);
    virtual void show() const;
    virtual void addInterest(const std::string& i);

    std::vector<std::string> getInterests() const;
    std::string getName() const;
    virtual ~User();


    bool operator>(const User& u) const;
    bool operator==(const User& u) const;
    friend std::ostream& operator<<(std::ostream& os, const User& u);

    friend void showUserPrivate(const User& u);
};

class PremiumUser : public User {
public:
    PremiumUser(std::string n);
    void show() const override;
    void addInterest(const std::string& i) override;
};

class VIPUser : public PremiumUser, public Logger {
public:
    VIPUser(std::string n);
    void show() const override;
};
#endif
