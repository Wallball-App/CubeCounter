#pragma once

#include <gmpxx.h>

class Calculation {
    public:
        int IDX;
        Calculation();
        mpz_class Calculate(int i);
        std::string returnString10(mpz_class i);
        std::string returnScientific(mpz_class i, int precision);
        int returnTrailingZeros(int i);
        int returnNumDigits(int i);
};