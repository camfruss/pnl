#pragma once

#include "trade.hpp"


struct Position
{
    int timestamp;
    double price;
    double quantity;

    Position() = default;

    Position(const TradeResponse& t_tradeResponse)
        : timestamp { t_tradeResponse.timestamp },
          price { t_tradeResponse.price },
          quantity { t_tradeResponse.quantity }
    {
    }

    bool operator<(const Position& t_position)
    {
        return price < t_position.price;
    }

    bool operator>(const Position& t_position)
    {
        return price > t_position.price;
    }
};

