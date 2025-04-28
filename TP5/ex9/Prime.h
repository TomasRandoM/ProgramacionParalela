#ifndef PRIME_H
#define PRIME_H
#include <mutex>
#include <vector>

class Prime {
    private:
    std::mutex mtx;

    public:

    Prime();
    bool isPrimeNumber(int number);
    void calculatePrimeNumbers(int from, int to, std::vector<int>& rta);

};

#endif