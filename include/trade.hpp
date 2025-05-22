#pragma once

#include <string>


enum class order_type;

struct trade {
    int timestamp;        // assumes date passed in as int
    std::string ticker;
    order_type orderType;
    double price;
    double quantity;      // to support fractional shares

    bool operator<(const trade& trade)
    {
        return price < trade.price;
    }

    bool operator>(const trade& trade)
    {
        return price > trade.price;
    }
};

