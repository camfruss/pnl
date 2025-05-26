#pragma once

#include "account.hpp"

#include <string>


struct Trade;

class Portfolio
{    
    public:
        /* Adds a trade to one's portfolio, calculating PnL if it closes out a previous position */
        virtual void add_trade(const Trade& t_trade) = 0;

    private:
        std::unordered_map<std::string, Account> m_portfolio {};
};

