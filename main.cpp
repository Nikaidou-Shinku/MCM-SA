#include "qwq.h"
#include "file.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <random>

using f64 = double;
using String = std::string;

constexpr int TOTAL_DAY{2000};
constexpr int SEED{998244353};
constexpr f64 eps{1e-9};

std::default_random_engine e(SEED);

bool less(f64 a, f64 b) {
    return a + eps < b;
}
bool equal(f64 a, f64 b) {
    return fabs(a - b) < eps;
}
bool lessOrEqual(f64 a, f64 b) {
    return a < b + eps;
}

std::ifstream fin_gold("GOLD-FIXED.csv");
std::ifstream fin_bcoin("BCHAIN-MKPRU.csv");

qwq::Worth getWorth[TOTAL_DAY];
file::Day getDay[TOTAL_DAY];

void printWorth(int tot) {
    for (int i = 1; i != tot; ++ i)
        std::cout << getDay[i] << ": " << getWorth[i] << "\n";
}

bool isOKToSale(int rhs) {
    return !equal(getDay[rhs - 1].getValue(), getDay[rhs].getValue());
}

std::vector<String> logger;

qwq::Money Summon(const qwq::Money& lhs,
        const qwq::Worth& rhs,
        const bool okToSale,
        const f64 kT) {

    qwq::Money res = lhs;
    std::uniform_real_distribution<f64> u(-1, 1);
    f64 pGold{u(e)}, pBcoin{u(e)};
    if (okToSale) {
        if (less(pGold, 0.)) {
            f64 tmp = pGold * lhs.getGold();
            res.addGold(tmp);
            tmp = abs(tmp) * 0.99 * rhs.getGold();
            res.addCash(tmp);
        } else {
            f64 tmp = pGold * lhs.getCash();
            res.addCash(-tmp);
            tmp /= rhs.getGold();
            res.addGold(tmp * 0.99);
        }
    }

    if (less(pBcoin, 0.)) {
        f64 tmp = pBcoin * lhs.getBitcoin();
        res.addBitcoin(tmp);
        tmp = abs(tmp) * 0.98 * rhs.getBitcoin();
        res.addCash(tmp);
    } else {
        f64 tmp = pBcoin * lhs.getCash();
        res.addCash(-tmp);
        tmp /= rhs.getBitcoin();
        res.addBitcoin(tmp * 0.98);
    }

    // if (less(res.getCash(), 0.)) {
    //     std::cout << "GG";
    //     assert(false);
    // }
    return res;
}

qwq::Money SA(qwq::Money now, int today, int tomorrow) {
    constexpr f64 _T{1e-8}, delta{0.99};
    bool okToSale{isOKToSale(today)};

    f64 maxT{1e6}, T{1e6};
    qwq::Money res = now;
    while (less(_T, T)) {
        qwq::Money tmp = Summon(now, getWorth[today], okToSale, T / maxT);
        f64 vNow{res * getWorth[tomorrow]};
        f64 vNew{tmp * getWorth[tomorrow]};

        if (less(vNow, vNew))
            res = tmp;
        else {
            std::uniform_real_distribution<f64> u(0, 1);
            f64 P{exp((vNew - vNow) / T)};
            if (lessOrEqual(u(e), P))
                res = tmp;
        }
        T *= delta;
    }
    std::cout << (res * getWorth[tomorrow]) << ": ";
    return res;
}

int main() {
    String s, t;
    std::getline(fin_gold, s);
    std::getline(fin_bcoin, t);

    int idx{1};
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

    // printWorth(idx);

    qwq::Money nowV(1000., 0., 0.);
    for (int i{1}; i + 1 != idx; ++ i) {
        qwq::Money adjust = SA(nowV, i, i + 1);
        std::cout << nowV << " -> " << adjust << "\n";
        nowV = adjust;
    }
    return 0;
}
