#pragma once

#include "../include/types.hpp"

#include <sstream>
#include <string>


class Trade;

template <typename Strategy>
class PnLCalculator
{
    public:
        /*
         *
         */
        void calculate(const std::string& input_file) const;

    private:
        /*
         *
         */
        Trade parse_trade(const std::string& line);
        
        /*
         *
         */
        void process_trade(const Trade& trade);

        /* Returns true if trade is able to close out a current position
         * and false if trade adds to a current position
         */
        inline bool can_close(const Trade& trade, PositionType position_type)
        {
            return (
                position_type == PositionType::NONE ||
                (trade.orderType == OrderType::BUY && position_type == PositionType::SHORT) ||
                (trade.orderType == OrderType::SELL && position_type == PositionType::LONG)
            );
        }

    private:
        std::unordered_map<std::string, std::pair<PositionType, Strategy>> m_openPositions {};
        std::ostringstream m_oss;
};
