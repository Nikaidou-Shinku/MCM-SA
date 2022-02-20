#include "qwq.h"

namespace qwq {

using f64 = double;
using String = std::string;

Money::Money(): cash(0.), gold(0.), bitcoin(0.) { }
Money::Money(f64 _cash, f64 _gold, f64 _bitcoin):
        cash(_cash), gold(_gold), bitcoin(_bitcoin) { }

f64 operator*(const Money& lhs, const Worth& rhs) {
    f64 res{0.};
    res += lhs.cash * rhs.cash;
    res += lhs.gold * rhs.gold;
    res += lhs.bitcoin * rhs.bitcoin;
    return res;
}

std::ostream& operator<<(std::ostream& out, const Money& rhs) {
    out << "(" << rhs.cash << ", " << rhs.gold << ", " << rhs.bitcoin << ")";
    return out;
}

double Money::getCash() const {
    return cash;
}
void Money::addCash(const double _cash) {
    cash += _cash;
}
double Money::getGold() const {
    return gold;
}
void Money::addGold(const double _gold) {
    gold += _gold;
}
double Money::getBitcoin() const {
    return bitcoin;
}
void Money::addBitcoin(const double _bitcoin) {
    bitcoin += _bitcoin;
}

} // end namespace qwq
