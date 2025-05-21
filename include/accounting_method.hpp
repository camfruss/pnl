#pragma once

class Position;
class Trade;


class AccountingMethod 
{    
    public:
        /*
         *
         */
        virtual void add_trade(const Trade&) = 0;

        /*
         *
         */
        virtual Position& peek() = 0;

        /*
         *
         */
        virtual void pop() = 0;
};

