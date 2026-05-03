#ifndef RULE_H
#define RULE_H
#include <string>

class Rule {
public:
    virtual std::string apply(const std::string& interest) = 0;
    virtual std::string getCond() const = 0;
    virtual std::string getRec() const = 0;
    virtual ~Rule() {}
};

class BasicRule : public Rule {
private:
    std::string cond, rec;
public:
    BasicRule(std::string c, std::string r);
    std::string apply(const std::string& interest) override;
    std::string getCond() const override;
    std::string getRec() const override;
};

class RuleFactory {
public:
    static Rule* createRule(std::string c, std::string r);
};
#endif
