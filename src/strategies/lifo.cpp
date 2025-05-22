#include "../../include/position.hpp"
#include "../../include/trade.hpp"

#include <stack>


class lifo : public position 
{    
    public:
        void update(const trade& t_trade) override {
            m_positions.push(t_trade);
        }

        trade& peek() override {
            return m_positions.top();
        }

        void pop() override {
            m_positions.pop();
            if (m_positions.empty())
            {
                m_positionType = position_type::none;
            }
        }

    private:
        std::stack<trade> m_positions;
};
