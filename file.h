#ifndef _FILE_H
#define _FILE_H

#include <iostream>
#include <string>

namespace file {

class Day {
  private:
    int y, m, d;
    double value;
    friend std::ostream& operator<<(std::ostream&, Day&);
  public:
    Day();
    Day(const int, const int, const int, const double);
    bool operator==(const Day&) const;
    void parseFromStr(std::string);
    bool isRun() const;
    bool lastDay() const;
    bool lastMonth() const;
    Day getNext() const;
    bool checkNext(const Day&) const;
    double getValue() const;
};

} // end namespace file

#endif
