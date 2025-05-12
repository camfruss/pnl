#include "../include/pnl_calculator.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>


void PnLCalculator::calculate(const std::string& csv_file, const std::string& str_strategy)
{
    std::ifstream ifs { csv_file };
    Strategy strategy { str_strategy == "fifo" ? Strategy::FIFO : Strategy::LIFO };
    std::string line {};
    
    std::getline(ifs, line);  // skips header line 
    while (ifs.good() && std::getline(ifs, line))
    {
        Trade trade {};
        parse_trade(line, trade);
        process_trade(trade, strategy);
    }

    std::cout << std::format("TIMESTAMP,SYMBOL,PNL\n");
    std::cout << m_oss.str() << std::endl;
}

void PnLCalculator::parse_trade(const std::string& line, Trade& trade)
{
    std::istringstream iss { line };
    std::string tmp {};  // seems inefficient
    
    std::getline(iss, tmp, ',');
    trade.timestamp = std::stoi(tmp);

    std::getline(iss, trade.ticker, ',');

    std::getline(iss, tmp, ',');
    trade.orderType = tmp == "B" ? OrderType::BUY : OrderType::SELL;

    std::getline(iss, tmp, ',');
    trade.price = std::stod(tmp);

    std::getline(iss, tmp);
    trade.quantity = std::stod(tmp);
}

void PnLCalculator::process_trade(const Trade& trade, Strategy strategy)
{
    auto& [position_type, positions] { m_openPositions[trade.ticker] };

    bool is_clearing { false };
    double qty_unmatched { trade.quantity }, trade_pnl {};
    while (qty_unmatched && !positions.empty() && can_close(trade, position_type))
    {
        switch (strategy) 
        {
            case Strategy::LIFO: {
                auto& order_to_close { positions.back() };
                if (trade.quantity - order_to_close.quantity >= -std::numeric_limits<double>::epsilon()) {
                    positions.pop_back();
                    is_clearing = true;
                } else {
                    order_to_close.quantity -= trade.quantity;
                }
                
                double abs_pnl { trade.quantity * (trade.price - order_to_close.price) };
                trade_pnl += trade.orderType == OrderType::SELL ? abs_pnl : -1 * abs_pnl;
                
                break;
            }
            case Strategy::FIFO: {
                auto& order_to_close { positions.front() };
                if (trade.quantity - order_to_close.quantity >= -std::numeric_limits<double>::epsilon()) {
                    positions.pop_front();
                    is_clearing = true;
                } else {
                    order_to_close.quantity -= trade.quantity;
                }
                
                double abs_pnl { trade.quantity * (trade.price - order_to_close.price) };
                trade_pnl += trade.orderType == OrderType::SELL ? abs_pnl : -1 * abs_pnl;
                
                break;
            }
        }
    }

    if (positions.empty()) {
        position_type = PositionType::NONE;
    }

    if (qty_unmatched > std::numeric_limits<double>::epsilon() || 
        !can_close(trade, position_type)) 
    {
        using enum PositionType;

        positions.emplace_back(trade.price, trade.quantity);
        position_type = trade.orderType == OrderType::BUY ? LONG : SHORT;
    }

    if (is_clearing) {
        m_oss << std::format("{},{},{}\n", trade.timestamp, trade.ticker, trade_pnl);
    }
}
