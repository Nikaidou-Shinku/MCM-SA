#include "file.h"

#include <cstdio>
#include <sstream>

namespace file {

using f64 = double;
using String = std::string;

constexpr int DAYS[] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

Day::Day() { }
Day::Day(const int _y, const int _m, const int _d, const f64 _value) {
    y = _y; m = _m; d = _d; value = _value;
}

std::ostream& operator<<(std::ostream& out, Day& rhs) {
    out << "[20" << rhs.y << "/" << rhs.m << "/" << rhs.d << "]";
    return out;
}

bool Day::operator==(const Day& rhs) const {
    if (y != rhs.y)
        return false;
    if (m != rhs.m)
        return false;
    if (d != rhs.d)
        return false;
    return true;
}

void Day::parseFromStr(String s) {
    char c;
    std::stringstream buf(s);
    buf >> m >> c >> d >> c >> y >> c >> value;
}

bool Day::isRun() const {
    return !(y & 3);
}

bool Day::lastDay() const {
    int daysThisMonth = DAYS[m % 12];
    if (isRun() && m == 2)
        ++ daysThisMonth;
    return d == daysThisMonth;
}

bool Day::lastMonth() const {
    return m == 12;
}

Day Day::getNext() const {
    Day nxtDay;
    if (lastMonth()) {
        if (lastDay()) {
            nxtDay = Day(y + 1, 1, 1, value);
        } else {
            nxtDay = Day(y, m, d + 1, value);
        }
    } else {
        if (lastDay()) {
            nxtDay = Day(y, m + 1, 1, value);
        } else {
            nxtDay = Day(y, m, d + 1, value);
        }
    }
    return nxtDay;
}

bool Day::checkNext(const Day& rhs) const {
    return getNext() == rhs;
}

f64 Day::getValue() const {
    return value;
}

} // end namespace file
