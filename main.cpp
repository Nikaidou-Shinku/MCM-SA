#include "qwq.h"
#include "file.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

using f64 = double;
using String = std::string;

constexpr int TOTAL_DAY = 2000;
constexpr f64 eps = 1e-9;

bool less(double a, double b) {
    return a + eps < b;
}

std::ifstream fin_gold("GOLD-FIXED.csv");
std::ifstream fin_bcoin("BCHAIN-MKPRU.csv");

qwq::Worth getWorth[TOTAL_DAY];
file::Day getDay[TOTAL_DAY];

void printWorth(int tot) {
    for (int i = 1; i != tot; ++ i)
        std::cout << getDay[i] << ": " << getWorth[i] << "\n";
}

std::vector<String> logger;

qwq::Money SA(qwq::Money now,
        const qwq::Worth& today,
        const qwq::Worth& tomorrow) {
    f64 T = 1e6, _T = 1e-6;

    while (less(_T, T)) {

    }
}

int main() {
    String s, t;
    std::getline(fin_gold, s);
    std::getline(fin_bcoin, t);

    int idx = 1;
    while (true) {
        std::getline(fin_gold, s);
        std::getline(fin_bcoin, t);

        if (fin_gold.eof() ^ fin_bcoin.eof())
            assert(false);
        
        if (fin_gold.eof())
            break;
        
        file::Day now_gold, now_bcoin;
        now_gold.parseFromStr(s);
        now_bcoin.parseFromStr(t);

        getDay[idx] = now_gold;
        getWorth[idx] = qwq::Worth(1., now_gold.getValue(), now_bcoin.getValue());

        ++ idx;
    }

    printWorth();

    qwq::Money atFirst(1000., 0., 0.);
    return 0;
}
