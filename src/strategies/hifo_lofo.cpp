#include "../../include/trade.hpp"
#include "../../include/position.hpp"

#include <queue>
#include <vector>


template <typename Comparator>
class inventory_heap : public position
{
    public:
        void update(const trade& t_trade) 
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

    private:
        std::priority_queue<trade, std::vector<trade>, Comparator> m_positions;
};


using hifo = inventory_heap<std::less<trade>()>;     // max heap
using lofo = inventory_heap<std::greater<trade>()>;  // min heap

