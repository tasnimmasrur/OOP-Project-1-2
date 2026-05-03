#include "Rule.h"

BasicRule::BasicRule(std::string c, std::string r) : cond(c), rec(r) {}

std::string BasicRule::apply(const std::string& interest) {
    return (interest == cond) ? rec : "";
}

std::string BasicRule::getCond() const { return cond; }
std::string BasicRule::getRec() const { return rec; }

Rule* RuleFactory::createRule(std::string c, std::string r) {
    return new BasicRule(c, r);
}
