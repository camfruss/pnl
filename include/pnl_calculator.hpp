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
        struct Positions {
            double price;
            double quantity;
        };
        
        struct Trade {
            int timestamp;  // assumes date passed in as int
            std::string ticker;
            std::string orderType;
            double price;
            int quantity;
        };

        enum class OrderType {
            Buy,
            Sell
        };

        enum class Strategy {
            FIFO,
            LIFO
        };

        void parse_trade(const std::string& line, Trade& trade);
        void process_trade(const Trade& trade, Strategy strategy);

    private:
        std::unordered_map<std::string, std::deque<Positions>> m_openPositions {};
        std::ostringstream m_oss;
};
