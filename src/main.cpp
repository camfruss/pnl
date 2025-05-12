#include "../include/pnl_calculator.hpp"


int main(int argc, char* argv[])
{
    const std::string csv_path { argv[1] };
    const std::string strategy { argv[2] }; 

    PnLCalculator pnl;
    pnl.calculate(csv_path, strategy);

    return 0;
}
