#include "qwq.h"
#include "file.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <string>
#include <random>

using f64 = double;
using String = std::string;

constexpr int TOTAL_DAY{2000};
// constexpr int SEED{114514};
constexpr f64 eps{1e-10};

// std::default_random_engine e(SEED);
std::random_device e;

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

f64 Normalization(const f64 p) {
    f64 res{p};
    if (less(res, -1.))
        res = -1.;
    if (less(1., res))
        res = 1.;
    return res;
}

f64 getP(const f64 p, const f64 kT) {
    std::uniform_real_distribution<f64> u(-kT, kT);
    f64 res{p + u(e)};
    return Normalization(res);
}

qwq::Money Summon(const qwq::Money& lhs,
        const qwq::Worth& rhs,
        const bool okToSale,
        const f64 pGold,
        const f64 pBcoin) {

    qwq::Money res = lhs;
    
    if (okToSale) {
        if (less(pGold, 0.)) {
            f64 tmp = pGold * res.getGold();
            res.addGold(tmp);
            tmp = abs(tmp) * 0.99 * rhs.getGold();
            res.addCash(tmp);
        } else {
            f64 tmp = pGold * res.getCash();
            // std::cout << res.getCash() << " - " << tmp << "\n";
            res.addCash(-tmp);
            tmp /= rhs.getGold();
            res.addGold(tmp * 0.99);
        }
    }

    if (less(pBcoin, 0.)) {
        f64 tmp = pBcoin * res.getBitcoin();
        res.addBitcoin(tmp);
        tmp = abs(tmp) * 0.98 * rhs.getBitcoin();
        res.addCash(tmp);
    } else {
        f64 tmp = pBcoin * res.getCash();
        // std::cout << res.getCash() << " - " << tmp << "\n";
        res.addCash(-tmp);
        tmp /= rhs.getBitcoin();
        res.addBitcoin(tmp * 0.98);
    }

    if (less(res.getCash(), 0.)) {
        // res.addCash(-res.getCash());
        std::cout << "GG";
        assert(false);
    }
    return res;
}

qwq::Money SA(qwq::Money now, int today, const qwq::Worth& tomorrow) {
    constexpr f64 _T{1e-2}, delta{0.99};
    bool okToSale{isOKToSale(today)};

    if (!okToSale)
        std::cout << "\033[0;31mNo Gold | ";
    else std::cout << "\033[mOK Gold | ";

    f64 T{1e8};
    qwq::Money res = now;
    f64 pBestGold{0.}, pBestBcoin{0.};

    while (less(_T, T)) {
        const f64 kT{(log10(T) + 2) / 10};
        f64 pGold{getP(pBestGold, kT)};
        f64 pBcoin{getP(pBestBcoin, kT)};
        qwq::Money tmp = Summon(now, getWorth[today], okToSale, pGold, pBcoin);
        f64 vNow{res * tomorrow};
        f64 vNew{tmp * tomorrow};

        if (less(vNow, vNew)) {
            res = tmp;
            pBestGold = pGold;
            pBestBcoin = pBcoin;
        } else {
            std::uniform_real_distribution<f64> u(0, 1);
            f64 P{exp((vNew - vNow) / T)};
            if (lessOrEqual(u(e), P)) {
                res = tmp;
                pBestGold = pGold;
                pBestBcoin = pBcoin;
            }
        }
        T *= delta;
    }
    f64 vNow{res * tomorrow};
    f64 vNew{now * tomorrow};
    if (less(vNow, vNew))
        res = now;
    std::cout << (res * getWorth[today + 1]) << ": ";
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
        std::uniform_real_distribution<f64> u(-10, 10);
        qwq::Worth expectTomorrow(1,
            getWorth[i + 1].getGold() + u(e),
            getWorth[i + 1].getBitcoin() + u(e));
        // qwq::Worth expectTomorrow(1,
        //     getWorth[i].getGold() * 2 - getWorth[i - 1].getGold(),
        //     getWorth[i].getBitcoin() * 2 - getWorth[i - 1].getBitcoin());
        qwq::Money adjust = SA(nowV, i, expectTomorrow);
        std::cout << getDay[i] << ": " << nowV << " -> " << adjust << "\n";
        nowV = adjust;
    }
    return 0;
}
