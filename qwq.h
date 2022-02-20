#ifndef _QWQ_H
#define _QWQ_H

#include <iostream>

namespace qwq {

class Money;
using Worth = Money;

class Money {
  private:
    double cash, gold, bitcoin;
    friend double operator*(const Money&, const Worth&);
    friend std::ostream& operator<<(std::ostream&, const Money&);
  public:
    Money();
    Money(double, double, double);

    double getCash() const;
    void addCash(const double);
    double getGold() const;
    void addGold(const double);
    double getBitcoin() const;
    void addBitcoin(const double);
};

} // end namespace qwq

#endif
