#pragma once

#include<deque>
#include <sstream>
#include <string>
#include <unordered_map>


class PnLCalculator
{
    public:
        void calculate(const std::string& input_file, const std::string& strategy);

    private:
        enum class OrderType {
            BUY,
            SELL
        };

        enum class PositionType {
            NONE,
            SHORT,
            LONG
        };

        enum class Strategy {
            FIFO,
            LIFO
        };

        struct Position {
            double price;
            double quantity;
        };
        
        struct Trade {
            int timestamp;  // assumes date passed in as int
            std::string ticker;
            OrderType orderType;
            double price;
            double quantity;  // to support fractional shares
        };

        void parse_trade(const std::string& line, Trade& trade);
        void process_trade(const Trade& trade, auto& peek, auto& pop);

        /* Returns true if {trade} is able to close out a current position
         * and false if {trade} adds to a current position
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
        std::unordered_map<std::string, std::pair<PositionType, std::deque<Position>>> m_openPositions {};
        std::ostringstream m_oss;
};
