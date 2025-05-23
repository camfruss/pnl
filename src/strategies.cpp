#include "../include/position.hpp"


void fifo::add_trade(const trade& t_trade) 
{
    m_positions.push(t_trade);
}

trade& fifo::peek() 
{
    return m_positions.front();
}

void fifo::pop() 
{
    m_positions.pop();
    if (m_positions.empty())
    {
        m_positionType = position_type::none;
    }
}

bool fifo::empty()
{
    return m_positions.empty();
}


void lifo::add_trade(const trade& t_trade) {
    m_positions.push(t_trade);
}

trade& lifo::peek() {
    return m_positions.top();
}

void lifo::pop() {
    m_positions.pop();
    if (m_positions.empty())
    {
        m_positionType = position_type::none;
    }
}

bool lifo::empty()
{
    return m_positions.empty();
}


void wac::add_trade(const trade& t_trade) {
    m_avgPrice = m_avgPrice * m_quantity + t_trade.price * t_trade.quantity;
    m_quantity += t_trade.quantity;
    m_avgPrice /= m_quantity;

    m_pseudoTrade.price = m_avgPrice;
    m_pseudoTrade.quantity = m_quantity;
}

trade& wac::peek() {
    return m_pseudoTrade; 
}

void wac::pop() 
{
    if (m_quantity < std::numeric_limits<double>::epsilon()) 
    {
        m_positionType = position_type::none;
    }
} 

bool wac::empty()
{
    return m_quantity < std::numeric_limits<double>::epsilon();
}

