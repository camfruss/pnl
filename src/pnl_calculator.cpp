#include "../include/pnl_calculator.hpp"
#include "../include/position.hpp"
#include "../include/trade.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>


template <typename T>
void pnl_calculator<T>::calculate(const std::string& t_csvFile)
{
    std::ifstream ifs { t_csvFile };
    std::string line {};

    std::getline(ifs, line);  // skips header line 
    while (ifs.good() && std::getline(ifs, line))
    {
        trade t { parse_trade(line) };
        process_trade(t);
    }

    std::cout << std::format("TIMESTAMP,SYMBOL,PNL\n");
    std::cout << m_oss.str() << std::endl;
}

template <typename T>
trade pnl_calculator<T>::parse_trade(const std::string& t_line)
{
    trade t {};
    std::istringstream iss { t_line };
    std::string tmp {};  // seems inefficient
    
    std::getline(iss, tmp, ',');
    t.timestamp = std::stoi(tmp);

    std::getline(iss, t.ticker, ',');

    std::getline(iss, tmp, ',');
    t.orderType = tmp == "B" ? order_type::buy : order_type::sell;

    std::getline(iss, tmp, ',');
    t.price = std::stod(tmp);

    std::getline(iss, tmp);
    t.quantity = std::stod(tmp);

    return t;
}

template <typename T>
void pnl_calculator<T>::process_trade(const trade& t_trade)
{
    auto& pos { m_openPositions[t_trade.ticker] };

    bool is_clearing { pos->can_close(t_trade) };
    double qty_unmatched { t_trade.quantity };
    double trade_pnl {};

    while (qty_unmatched > 0 && pos->can_close(t_trade))
    {
        double shares_matched {};
        auto& order_to_close { pos->peek() };

        if (qty_unmatched - order_to_close.quantity >= -std::numeric_limits<double>::epsilon())  // full close
        { 
            shares_matched = order_to_close.quantity;
            pos->pop();
        } 
        else  // partial close  
        { 
            shares_matched = qty_unmatched;
            qty_unmatched = 0;
            order_to_close.quantity -= shares_matched;
        }
       
        double abs_pnl { shares_matched * (t_trade.price - order_to_close.price) };
        trade_pnl += t_trade.orderType == order_type::sell ? abs_pnl : -1 * abs_pnl;
        qty_unmatched -= shares_matched; 
    }

    // move logic into update ... if goes from 
    if (qty_unmatched > std::numeric_limits<double>::epsilon() || !pos->can_close(t_trade)) 
    {
        m_positionType = t_trade.orderType == order_type::buy ? position_type::long : position_type::short;
        pos->update(t_trade.price, qty_unmatched);
    }

    if (is_clearing) 
    {
        m_oss << std::format("{},{},{:.2f}\n", trade.timestamp, trade.ticker, trade_pnl);
    }
}
