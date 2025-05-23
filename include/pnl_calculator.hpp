#pragma once

#include "../include/position.hpp"
#include "../include/trade.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>


template <typename T>
concept AccountingMethod = requires { std::is_base_of_v<position, T>; };

template <AccountingMethod T>
class pnl_calculator
{
    public:
        /*
         *
         */
        void calculate(const std::string&);

    private:
        /*
         *
         */
        trade parse_trade(const std::string& t_line);
        
        /*
         *
         */
        void process_trade(const trade& t_trade);

    private:
        std::unordered_map<std::string, std::unique_ptr<T>> m_openPositions {};
        std::ostringstream m_oss;
};


template <AccountingMethod T>
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

template <AccountingMethod T>
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

template <AccountingMethod T>
void pnl_calculator<T>::process_trade(const trade& t_trade)
{
    auto& pos { m_openPositions[t_trade.ticker] };
    if (!pos)  // on first access, unique_ptr default ctor sets ptr to nullptr (vs. default ctor for T) 
    {
        pos = std::make_unique<T>();
    }

    bool is_clearing { pos->can_close(t_trade) };
    double unfilled { t_trade.quantity };
    double trade_pnl {};

    while (unfilled > 0 && pos->can_close(t_trade))
    {
        double filled {};
        trade& order_to_close { pos->peek() };

        if (unfilled - order_to_close.quantity >= -std::numeric_limits<double>::epsilon())  // full close
        { 
            filled = order_to_close.quantity;
            pos->pop();
        } 
        else  // partial close  
        { 
            order_to_close.quantity -= unfilled;
            filled = unfilled;
        }

        // TODO: order_to_close CANNOT be used after pop !!

        double abs_pnl { filled * (t_trade.price - order_to_close.price) };
        trade_pnl += t_trade.orderType == order_type::sell ? abs_pnl : -1 * abs_pnl;
        unfilled -= filled; 
    }

    // true if position transitions from short -> long OR long -> short
    // effective quantity in trade is actual - filled == unfilled 
    if (unfilled > std::numeric_limits<double>::epsilon() || !pos->can_close(t_trade)) 
    {
        trade t { t_trade };
        t.quantity = unfilled;
        pos->add_trade(t);
    }

    if (is_clearing) 
    {
        m_oss << std::format("{},{},{:.2f}\n", t_trade.timestamp, t_trade.ticker, trade_pnl);
    }
}

