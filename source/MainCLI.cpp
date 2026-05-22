#include <gmpxx.h>
#include <iostream>
#include <string>
#include <Calculation.h>

int LAYERS;
bool PRINTFULLNUMBER;

static mpz_class number;
static std::string numcombos;
static std::string numcombos_scientific;
static std::string numdigits;
static std::string numtrailzeros;


int main(int argc, char* argv[]) {
    for(int i=0; i<argc; i++) {
        std::string arg = argv[i];
        if(arg == "--layercount") {
            LAYERS = std::stoi(argv[i+1]);
        }
        if(arg == "--printfullnumber") {
            std::string arg1 = argv[i+1];
            PRINTFULLNUMBER = (arg1 == "true" || arg1 == "1");
        }
    }

    Calculation calculation = Calculation();

    std::cout<<"Calculating Number Of Combinations [1/5]"<<std::endl;
    number = calculation.Calculate(LAYERS);

    std::cout<<"Converting to String [2/5]"<<std::endl;
    if(calculation.returnNumDigits(LAYERS) < 50 || PRINTFULLNUMBER) numcombos = number.get_str();
    else std::cout<<"[Info] Skipping Conversion. Use '--printfullnumber 1' or '--printfullnumber true' to print the entire number"<<std::endl;

    std::cout<<"Calculating Scientific Notation and Formatting [3/5]"<<std::endl;
    numcombos_scientific = calculation.returnScientific(number, 4);

    std::cout<<"Calculating Number of Digits [4/5]"<<std::endl;
    numdigits = std::to_string(calculation.returnNumDigits(LAYERS));

    std::cout<<"Calculating Number Of Trailing Zeros [5/5]"<<std::endl;
    numtrailzeros = std::to_string(calculation.returnTrailingZeros(LAYERS));
    std::cout<<"-----------------------------------------"<<std::endl;



    std::cout<<"Calculation Finished Successfully"<<std::endl;
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout<<"Number of Combinations: "<<std::endl;
    if(calculation.returnNumDigits(LAYERS) < 50 || PRINTFULLNUMBER) {
        std::cout<<numcombos.c_str()<<std::endl;
    } else {
        std::cout<<"[Info] Number too large to display fully."<<std::endl;
    }
    std::cout<<"Number of Combinations (Scientific): "<<std::endl;
    std::cout<<numcombos_scientific.c_str()<<std::endl;

    std::cout<<"Number of Digits: "<<std::endl;
    std::cout<<numdigits.c_str()<<std::endl;

    std::cout<<"Number of Trailing Zeros: "<<std::endl;
    std::cout<<numtrailzeros.c_str()<<std::endl;
}