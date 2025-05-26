#include "../include/account.hpp"
#include "../include/portfolio.hpp"


void Portfolio::add_trade(const Trade& t_trade)
{
    Account& account { m_portfolio[t_trade.ticker] };
    account.add_trade(t_trade);
}
