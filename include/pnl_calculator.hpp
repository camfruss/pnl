#pragma once

#include "../include/trade.hpp"
#include "../include/types.hpp"

#include <sstream>
#include <string>


class position;

template <typename T>
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
        std::unordered_map<std::string, std::unique_ptr<position>> m_openPositions {};
        std::ostringstream m_oss;
};
