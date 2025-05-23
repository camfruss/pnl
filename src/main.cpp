#include "../include/pnl_calculator.hpp"
#include "../include/position.hpp"


int main(int argc, char* argv[])
{
    const std::string csv_path { argv[1] };
    const std::string strategy { argv[2] }; 

    pnl_calculator<fifo> pnl;
    pnl.calculate(csv_path);

    return 0;
}
