
enum class OrderType {
    BUY,
    SELL
};

enum class PositionType {
    NONE,
    SHORT,
    LONG
};

enum class StrategyType {
    FIFO,  // first-in, first-out
    LIFO,  // last-in, first-out 
    HIFO,  // highest-in, first-out 
    LOFO,  // lowest-in, first-out
    WAC    // weighted average cost
};

