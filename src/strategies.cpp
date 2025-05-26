#include "../include/account.hpp"


TradeResponse Account::update_position(const Trade& t_trade) 
{
    bool is_clearing { can_close(t_trade) };
    double unfilled { t_trade.quantity };
    double trade_pnl {};

    while (unfilled > 0 && can_close(t_trade))
    {
        bool should_pop { false };
        double filled {};
        Position& order_to_close { peek() };

        if (unfilled - order_to_close.quantity >= -std::numeric_limits<double>::epsilon())  // full close
        { 
            filled = order_to_close.quantity;
            should_pop = true;
        } 
        else  // partial close  
        { 
            order_to_close.quantity -= unfilled;
            filled = unfilled;
        }

        double abs_pnl { filled * (t_trade.price - order_to_close.price) };
        trade_pnl += t_trade.orderType == OrderType::sell ? abs_pnl : -1 * abs_pnl;
        unfilled -= filled;

        if (should_pop) pop();
    }

    // true if position transitions from short -> long OR long -> short
    // effective quantity in trade is actual - filled == unfilled 
    if (unfilled > std::numeric_limits<double>::epsilon() || !can_close(t_trade)) 
    {
        Trade t { t_trade };
        t.quantity = unfilled;
        add_trade(t);
    }

    TradeResponse result = {
        .isClearing = is_clearing, 
        .timestamp = t_trade.timestamp,
        .price = t_trade.price,
        .quantity = unfilled,
        .ticker = t_trade.ticker,
        .pnl = trade_pnl
    };
    return result;
}


void FIFO::add_trade(const Trade& t_trade) 
{ // trade -> position
    TradeResponse tr { update_position(t_trade) };
    m_positions.push(tr);
}

Position& FIFO::peek() 
{
    return m_positions.front();
}

void FIFO::pop() 
{
    m_positions.pop();
    if (m_positions.empty())
    {
        m_positionType = PositionType::none;
    }
}

bool FIFO::empty()
{
    return m_positions.empty();
}


void LIFO::add_trade(const Trade& t_trade) {
    TradeResponse tr { update_position(t_trade) };
    m_positions.push(tr);
}

Position& LIFO::peek() {
    return m_positions.top();
}

void LIFO::pop() {
    m_positions.pop();
    if (m_positions.empty())
    {
        m_positionType = PositionType::none;
    }
}

bool LIFO::empty()
{
    return m_positions.empty();
}


void WeightedAverageCost::add_trade(const Trade& t_trade) {
    m_avgPrice = m_avgPrice * m_quantity + t_trade.price * t_trade.quantity;
    m_quantity += t_trade.quantity;
    m_avgPrice /= m_quantity;

    m_pseudoPosition.price = m_avgPrice;
    m_pseudoPosition.quantity = m_quantity;
}

Position& WeightedAverageCost::peek() {
    return m_pseudoPosition; 
}

void WeightedAverageCost::pop() 
{
    if (m_quantity < std::numeric_limits<double>::epsilon()) 
    {
        m_positionType = PositionType::none;
    }
} 

bool WeightedAverageCost::empty()
{
    return m_quantity < std::numeric_limits<double>::epsilon();
}

