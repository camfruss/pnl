#pragma once

#include "position.hpp"
#include "types.hpp"
#include "trade.hpp"

#include <queue>
#include <stack>
#include <vector>


class Account 
{    
    public:
        virtual ~ Account() = default;

        /* Adds a trade to one's portfolio, calculating PnL if it closes out a previous position */
        virtual void add_trade(const Trade& t_trade) = 0;

    protected:
        PositionType m_positionType { PositionType::none };
    
    protected:
        virtual Position& peek() = 0;

        virtual void pop() = 0;

        virtual bool empty() = 0;

        /* Implements the logic for add_trade ... */
        virtual TradeResponse update_position(const Trade& t_trade);

        /* Returns false if t_trade adds to a current position, and true otherwise */
        inline bool can_close(const Trade& t_trade)
        {
            return (!empty() && (
                m_positionType == PositionType::none ||
                (t_trade.orderType == OrderType::buy && m_positionType == PositionType::short_position) ||
                (t_trade.orderType == OrderType::sell && m_positionType == PositionType::long_position)
            ));
        }
};


class FIFO : public Account
{
    public:
        void add_trade(const Trade& t_trade) override;
        Position& peek() override;
        void pop() override;
        bool empty() override;

    private:
        std::queue<Position> m_positions {};
};


class LIFO : public Account
{
    public:
        void add_trade(const Trade& t_trade) override;
        Position& peek() override;
        void pop() override;
        bool empty() override;

    private:
        std::stack<Position> m_positions {};
};


template <typename Comparator>
class InventoryHeap : public Account
{
    public:
        void add_trade(const Trade& t_trade) override
        {
            TradeResponse tr { update_position(t_trade) };
            m_positions.push(tr);
        }

        Position& peek() override
        {
            return m_positions.top();
        }

        void pop() override
        {
            m_positions.pop();
            if (m_positions.empty())
            {
                m_positionType = PositionType::none;
            }
        }
        
        bool empty() override
        {
            return m_positions.empty();
        }

    private:
        std::priority_queue<Position, std::vector<Position>, Comparator> m_positions {};
};

using HIFO = InventoryHeap<std::less<Position>()>;     // max heap
using LOFO = InventoryHeap<std::greater<Position>()>;  // min heap


class WeightedAverageCost : public Account
{
    public:
        void add_trade(const Trade& t_trade) override;
        Position& peek() override;
        void pop() override;
        bool empty() override;

    private:
        double m_avgPrice {};
        double m_quantity {};
        Position m_pseudoPosition {};
};

