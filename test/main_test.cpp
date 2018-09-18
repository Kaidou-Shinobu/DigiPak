#include <iostream>

enum class DIGIPAKERRORCODE {
    DIGIPAKSUCCESS = 0,
    DIGIPAKFAIL = 1
};

int tests(){
    return static_cast<int>(DIGIPAKERRORCODE::DIGIPAKSUCCESS);
}

int main(){
    return tests();
}
