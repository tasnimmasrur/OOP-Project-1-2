#include "User.h"
#include "Exception.h"

int User::totalUsers = 0;

User::User(std::string n) {
    this->name = n;
    this->interests = new std::vector<std::string>();
    totalUsers++;
}

User::User(const User& u) {
    this->name = u.name;
    this->interests = new std::vector<std::string>(*u.interests);
}

void User::show() const {
    std::cout << "User: " << name << std::endl;
}

void User::addInterest(const std::string& i) {
    if (interests->size() >= 3) {
        throw MyException("Limit reached! Normal users can add max 3 interests. Upgrade to VIP!");
    }
    interests->push_back(i);
}

std::vector<std::string> User::getInterests() const { return *interests; }
std::string User::getName() const { return name; }

User::~User() {
    delete interests;
    totalUsers--;
}


bool User::operator>(const User& u) const {
    return interests->size() > u.interests->size();
}


bool User::operator==(const User& u) const {
    return this->name == u.name;
}


std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "[User: " << u.name << " | Interests: " << u.interests->size() << "]";
    return os;
}


void showUserPrivate(const User& u) {
    std::cout << "[Friend Access] Secret Info of: " << u.name << std::endl;
}

PremiumUser::PremiumUser(std::string n) : User(n) {}

void PremiumUser::show() const {
    std::cout << "Premium User: " << name << std::endl;
}

void PremiumUser::addInterest(const std::string& i) {
    interests->push_back(i);
}

VIPUser::VIPUser(std::string n) : PremiumUser(n) {
    log("VIP User " + n + " created.");
}

void VIPUser::show() const {
    std::cout << "[VIP] User: " << name << std::endl;
}
