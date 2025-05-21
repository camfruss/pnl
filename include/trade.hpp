#pragma once

#include <string>


enum class OrderType;

struct Trade {
    int timestamp;  // assumes date passed in as int
    std::string ticker;
    OrderType orderType;
    double price;
    double quantity;  // to support fractional shares
};

