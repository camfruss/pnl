#pragma once

#include "types.hpp"
#include "trade.hpp"


class position 
{    
    public:
        /*
         *
         */
        virtual void update(const trade& t_trade) = 0;

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

    protected:
        position_type m_positionType;

    private:
        /* Returns false if t_trade adds to a current position, and true otherwise */
        inline bool can_close(const trade& t_trade)
        {
            return (
                m_positionType == position_type::none ||
                (t_trade.orderType == order_type::buy && m_positionType == position_type::short_position) ||
                (t_trade.orderType == order_type::sell && m_positionType == position_type::long_position)
            );
        }
};

