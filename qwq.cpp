#include "qwq.h"

#include <random>

namespace qwq {

using f64 = double;
using String = std::string;

constexpr int SEED = 998244353;
std::default_random_engine e(SEED);

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

Money Summon(const Money& rhs) {
    Money res;

    return res;
}

} // end namespace qwq
