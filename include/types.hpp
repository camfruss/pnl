#pragma once


enum class OrderType {
    buy,
    sell
};

enum class PositionType {
    none,
    short_position,
    long_position
};

enum class AccountingType {
    fifo,  // first-in, first-out
    lifo,  // last-in, first-out 
    hifo,  // highest-in, first-out 
    lofo,  // lowest-in, first-out
    wac    // weighted average cost
};

