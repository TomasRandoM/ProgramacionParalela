#include "Prime.h"
#include <vector> 
#include <algorithm>
#include <cmath>
#include <iostream>

Prime::Prime() {}

bool Prime::isPrimeNumber(int number) {
    if (number == 1 || number == 0) {
        return false;
    }
    
    int numberSqrt = (int) std:: floor(std:: sqrt(number));
    
    for (int i = 2; i <= numberSqrt; i++) {
        if ((number % i) == 0) {
            return false;
        }  
    }
    return true;
}

void Prime::calculatePrimeNumbers(int from, int to, std::vector<int>& rta) {

    for (int i = from; i < to; i++){
        if (isPrimeNumber(i)) {
            rta.push_back(i);
        }
    }

}