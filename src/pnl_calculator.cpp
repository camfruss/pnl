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
   
    auto peek = [&strategy](std::deque<Position>& positions) {
        switch (strategy) {
            case Strategy::FIFO:
                return positions.front();
                break;
            case Strategy::LIFO:
                return positions.back();
                break;
        }
    };
    auto pop = [&strategy](std::deque<Position>& positions) {
        switch (strategy) {
            case Strategy::FIFO:
                return positions.pop_front();
                break;
            case Strategy::LIFO:
                return positions.pop_back();
                break;
        }
    };

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
        double shares_matched {};

        switch (strategy) 
        {
            case Strategy::LIFO: {
                auto& order_to_close { positions.back() };

                if (qty_unmatched - order_to_close.quantity >= -std::numeric_limits<double>::epsilon()) { // full close
                    positions.pop_back();
                    is_clearing = true;
                    shares_matched = order_to_close.quantity;
                    qty_unmatched -= order_to_close.quantity;
                } else {  // partial close 
                    shares_matched = qty_unmatched;
                    qty_unmatched = 0;
                    order_to_close.quantity -= shares_matched;
                }
               
                double abs_pnl { shares_matched * (trade.price - order_to_close.price) };
                trade_pnl += trade.orderType == OrderType::SELL ? abs_pnl : -1 * abs_pnl;
                
                break;
            }
            case Strategy::FIFO: {
                auto& order_to_close { positions.front() };

                if (qty_unmatched - order_to_close.quantity >= -std::numeric_limits<double>::epsilon()) { // full close
                    positions.pop_front();
                    is_clearing = true;
                    shares_matched = order_to_close.quantity;
                    qty_unmatched -= order_to_close.quantity;
                } else {  // partial close 
                    shares_matched = qty_unmatched;
                    qty_unmatched = 0;
                    order_to_close.quantity -= shares_matched;
                }
               
                double abs_pnl { shares_matched * (trade.price - order_to_close.price) };
                trade_pnl += trade.orderType == OrderType::SELL ? abs_pnl : -1 * abs_pnl;
                
                break;
            }

            qty_unmatched -= shares_matched;
        }
    }

    if (positions.empty()) {
        position_type = PositionType::NONE;
    }

    if (qty_unmatched > std::numeric_limits<double>::epsilon() || 
        !can_close(trade, position_type)) 
    {
        positions.emplace_back(trade.price, qty_unmatched);
        position_type = trade.orderType == OrderType::BUY ? PositionType::LONG : PositionType::SHORT;
    }

    if (is_clearing) {
        m_oss << std::format("{},{},{:.2f}\n", trade.timestamp, trade.ticker, trade_pnl);
    }
}
