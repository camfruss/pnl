#pragma once

#include "../include/portfolio.hpp"
#include "../include/trade.hpp"

#include <sstream>
#include <string>


class PnLCalculator
{
    public:
        explicit PnLCalculator(const std::string& t_accountingType);

        /*  */
        void calculate(const std::string& t_csvFile);

    private:
        /*  */
        Trade parse_trade(const std::string& t_line);
        
    private:
        std::ostringstream m_oss;
        std::unique_ptr<Portfolio> m_portfolio;
};
