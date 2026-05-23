#include <Calculation.h>
#include <cmath>
#include <string>
#include <format>

mpz_class Factorial24;
mpz_class Factorial12;
mpz_class Fact7X3to6;

Calculation::Calculation() {
    Factorial24 = factorial(24_mpz);
    Factorial12 = factorial(12_mpz);
    Fact7X3to6 = factorial(7_mpz) * (int)pow(3, 6); 
}
mpz_class Calculation::Calculate(int i) {
    if(i == 1) return 1_mpz;
    mpz_class Base = 24_mpz * (1<<10) * Factorial12;
    
    mpz_class OddComponent;
    mpz_pow_ui(OddComponent.get_mpz_t(), Base.get_mpz_t(), i%2);

    mpz_class FlooredComponent;
    mpz_pow_ui(FlooredComponent.get_mpz_t(), Factorial24.get_mpz_t(), floor((i-2)/2));

    mpz_class FinalComponent;
    mpz_class FinalBase;
    mpz_class Fact4to6;
    mpz_pow_ui(Fact4to6.get_mpz_t(), ((mpz_class)factorial(4_mpz)).get_mpz_t(), 6);
    FinalBase = Factorial24 / Fact4to6;
    mpz_pow_ui(FinalComponent.get_mpz_t(), FinalBase.get_mpz_t(), floor(pow((i-2)/2.0, 2)));

    mpz_class FinalNumber = Fact7X3to6 * OddComponent * FlooredComponent * FinalComponent;
    return FinalNumber;
}

std::string Calculation::returnString10(mpz_class i) {
    return i.get_str(10);
}
std::string Calculation::returnScientific(mpz_class i, int precision) {
    if(i == 1_mpz) return "1E0";
    mpf_class f(i, (precision+10) * 4);
    mp_exp_t exp;

    std::string str = f.get_str(exp, 10, precision+1);

    bool negative = (str[0] == '-');
    if(negative) str = str.substr(1);
    std::string result = (negative ? "-" : "");
    result += str[0];
    result += '.';
    if(str.length() > 1) {
        result += str.substr(1, precision);
    } 
    result += "E" + std::to_string(exp-1);
    return result;
}
int Calculation::returnNumDigits(int i) {
    if(i == 1) return 1;
    double a = 6.56515806476;
    double b = 13.0708481624;
    double c = 23.7927056702;
    double d = 15.5114382199;

    return floor(a + (i%2)*b + std::floor((i-2)/2)*c + std::floor(pow((i-2)/2.0,2))*d) + 1;
}
int Calculation::returnTrailingZeros(int i) {
    if(i == 1) return 0;
    return std::pow(i-((i%2) + 1), 2) + 2*(i%2);
}
