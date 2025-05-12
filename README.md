# PnL Calculator

## Usage
```
cmake -B build -S .
cmake --build ./build
./build/pnl_calculator <csv-path> <fifo|lifo>
```

## TODO
- [ ] Abstract out FIFO/LIFO accounting ... possibly use Strategy design pattern
- [ ] Add gtest test cases
