
enum class Strategy {
    FIFO,
    LIFO
};

enum class OrderType {
    Buy,
    Sell
};

class PnLCalculator
{
    public:
        void calculate(std::string_view input_file, Strategy strategy);

    private:
        struct Positions {
            double price;
            double quantity;
        };
        
        struct Trade {
            int timestamp;  // assumes date passed in as int
            std::string ticker;
            char orderType;
            double price;
            int quantity;
        };

        Trade parse_trade(const std::string& line);
        void calculate_trade(const Trade& trade);

    private:
        std::unordered_map<string, std::deque<Positions>> m_openPositions {};
};

int main(int argc, char* argv[])
{
    const std::string csv_path { argv[1] };
    const std::string arg2 { argv[2] }; 
    const Strategy strategy { strategy == "fifo" ? Strategy::FIFO : Strategy::LIFO };

    std::ifstream ifs { csv_path };

    std::ostringstream oss;
    std::string line {};

    while (std::cin.good() && std::getline(std::cin, line))
    {
        std::istringstream iss { line };
    
        Trade trade {};
        std::getline(iss, trade.timestamp, ',');
        std::getline(iss, trade.ticker, ',');
        std::getline(iss, trade.orderType, ',');
        std::getline(iss, trade.price, ',');
        std::getline(iss, trade.quantity, ',');
    }
    
    return 0;
}
