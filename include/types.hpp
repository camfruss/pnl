#pragma once


enum class order_type {
    buy,
    sell
};

enum class position_type {
    none,
    short_position,
    long_position
};

enum class accounting_method {
    fifo,  // first-in, first-out
    lifo,  // last-in, first-out 
    hifo,  // highest-in, first-out 
    lofo,  // lowest-in, first-out
    wac    // weighted average cost
};

