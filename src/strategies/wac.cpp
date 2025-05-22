#include "../../include/position.hpp" 
#include "../../include/trade.hpp" 


class wac : public position
{
    public:    
        void update(const trade& t_trade) {
            m_avgPrice = m_avgPrice * m_quantity + t_trade.price * t_trade.quantity;
            m_quantity += t_trade.quantity;
            m_avgPrice /= m_quantity;

            m_pseudoTrade.price = m_avgPrice;
            m_pseudoTrade.quantity = m_quantity;
        }

        trade& peek() {
            return m_pseudoTrade; 
        }

        void pop() 
        {
            if (m_quantity < std::numeric_limits<double>::epsilon()) 
            {
                m_positionType = position_type::none;
            }
        } 

    private:
        double m_avgPrice {};
        double m_quantity {};
        trade m_pseudoTrade {};
};

