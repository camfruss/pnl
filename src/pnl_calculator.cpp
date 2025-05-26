#include "../include/pnl_calculator.hpp"

#include <format>
#include <fstream>
#include <iostream>


void PnLCalculator::calculate(const std::string& t_csvFile)
{
    std::ifstream ifs { t_csvFile };
    std::string line {};

    std::getline(ifs, line);  // skips header line 
    while (ifs.good() && std::getline(ifs, line))
    {
        Trade t { parse_trade(line) };
        TradeResponse tr { m_portfolio->add_trade(t)};

        if (tr.isClearing) 
        {
            m_oss << std::format("{},{},{:.2f}\n", t.timestamp, t.ticker, tr.pnl);
        }
    }

    std::cout << std::format("TIMESTAMP,SYMBOL,PNL\n");
    std::cout << m_oss.str() << std::endl;
}

Trade PnLCalculator::parse_trade(const std::string& t_line)
{
    Trade t {};
    std::istringstream iss { t_line };
    std::string tmp {};  // seems inefficient
    
    std::getline(iss, tmp, ',');
    t.timestamp = std::stoi(tmp);

    std::getline(iss, t.ticker, ',');

    std::getline(iss, tmp, ',');
    t.orderType = tmp == "B" ? OrderType::buy : OrderType::sell;

    std::getline(iss, tmp, ',');
    t.price = std::stod(tmp);

    std::getline(iss, tmp);
    t.quantity = std::stod(tmp);

    return t;
}

