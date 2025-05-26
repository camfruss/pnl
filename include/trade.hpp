#pragma once

#include <string>


enum class OrderType;

struct Trade 
{
    int timestamp;        // assumes date passed in as int
    std::string ticker;
    OrderType orderType;
    double price;
    double quantity;      // to support fractional shares

    bool operator<(const Trade& t_trade)
    {
        return price < t_trade.price;
    }

    bool operator>(const Trade& t_trade)
    {
        return price > t_trade.price;
    }
};


struct TradeResponse
{
    bool isClearing;
    int timestamp;
    double price;
    double quantity;
    std::string ticker;
    double pnl;
};

