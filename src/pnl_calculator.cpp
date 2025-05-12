#include "../include/pnl_calculator.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>


void PnLCalculator::calculate(const std::string& csv_file, const std::string& str_strategy)
{
    std::ifstream ifs { csv_file };
    Strategy strategy { str_strategy == "fifo" ? Strategy::FIFO : Strategy::LIFO};
    std::string line {};
    
    std::getline(ifs, line);  // skips header line 
    while (ifs.good() && std::getline(ifs, line))
    {
        Trade trade {};
        parse_trade(line, trade);
        process_trade(trade, strategy);
    }

    std::cout << m_oss.str() << std::endl;
}

void PnLCalculator::parse_trade(const std::string& line, Trade& trade)
{
    std::istringstream iss { line };
    std::string tmp {};  // seems inefficient
    
    std::getline(iss, tmp, ',');
    trade.timestamp = std::stoi(tmp);

    std::getline(iss, trade.ticker, ',');

    std::getline(iss, trade.orderType, ',');
    
    std::getline(iss, tmp, ',');
    trade.price = std::stod(tmp);

    std::getline(iss, tmp);
    trade.quantity = std::stoi(tmp);
}

void PnLCalculator::process_trade(const Trade& trade, Strategy strategy)
{
    auto& positions { m_openPositions[trade.ticker] };
    
    int qty_unmatched { trade.quantity }, trade_pnl {};
    while (qty_unmatched && !positions.empty())
    {
        
    }

    // only if clearing
    m_oss << std::format("{},{},{}\n", trade.timestamp, trade.ticker, trade_pnl);
}
