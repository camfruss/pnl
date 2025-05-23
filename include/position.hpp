#pragma once

#include "types.hpp"
#include "trade.hpp"

#include <queue>
#include <stack>
#include <vector>


class position 
{    
    public:
        virtual ~ position() = default;

        /*
         *
         */
        virtual void add_trade(const trade& t_trade) = 0;

        /*
         *
         */
        virtual trade& peek() = 0;

        /*
         *
         */
        virtual void pop() = 0;

        /*
         *
         */
        virtual bool empty() = 0;

        /* Returns false if t_trade adds to a current position, and true otherwise */
        inline bool can_close(const trade& t_trade)
        {
            return (!empty() && (
                m_positionType == position_type::none ||
                (t_trade.orderType == order_type::buy && m_positionType == position_type::short_position) ||
                (t_trade.orderType == order_type::sell && m_positionType == position_type::long_position)
            ));
        }

    protected:
        position_type m_positionType { position_type::none };
};


class fifo : public position
{
    public:
        void add_trade(const trade& t_trade) override;
        trade& peek() override;
        void pop() override;
        bool empty() override;

    private:
        std::queue<trade> m_positions;
};


class lifo : public position 
{
    public:
        void add_trade(const trade& t_trade) override;
        trade& peek() override;
        void pop() override;
        bool empty() override;

    private:
        std::stack<trade> m_positions;
};


template <typename Comparator>
class inventory_heap : public position
{
    public:
        void add_trade(const trade& t_trade) override
        {
            m_positions.push(t_trade);
        }

        trade& peek() override
        {
            return m_positions.top();
        }

        void pop() override
        {
            m_positions.pop();
            if (m_positions.empty())
            {
                m_positionType = position_type::none;
            }
        }
        
        bool empty() override
        {
            return m_positions.empty();
        }

    private:
        std::priority_queue<trade, std::vector<trade>, Comparator> m_positions;
};

using hifo = inventory_heap<std::less<trade>()>;     // max heap
using lofo = inventory_heap<std::greater<trade>()>;  // min heap


class wac : public position
{
    public:
        void add_trade(const trade& t_trade) override;
        trade& peek() override;
        void pop() override;
        bool empty() override;

    private:
        double m_avgPrice {};
        double m_quantity {};
        trade m_pseudoTrade {};
};

