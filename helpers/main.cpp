#include <iostream>
#include <bitset>
#include <cstdint>
#include <math.h>

uint8_t binaryToDecimal(std::bitset<8> number)
{
    uint8_t result = 0;

    if(!number.none()){
        for(int i = 0; i < 8; i++){
            uint8_t exponent = std::pow(2, i);
            result += number[i] * std::pow(2, i);
        }
    }

    return result;
}

template <size_t N>
std::bitset<N> ode(std::bitset<N> a, std::bitset<N> b)
{
    while (b.any()){
        std::bitset<N> borrow = ~a & b; // NOT A AND B
        a = a ^ b; // A XOR B
        b = borrow << 1; // borrow shift right by 1
    }
    return a;
}

template <size_t N>
std::bitset<N> dod(std::bitset<N> a, std::bitset<N> b)
{
    // while (b.any()){
    //     std::bitset<N> borrow = ~a & b; // NOT A AND B
    //     a = a ^ b; // A XOR B
    //     b = borrow << 1; // borrow shift right by 1
    // }
    return a |= b;
}

int main(){
    std::bitset<8> a("11111111");
    std::bitset<8> b("11111111");

    std::cout << "Wynik: " << static_cast<int>(binaryToDecimal(dod(a, b))) << "\n";
    // std::cout << "Wynik: " << static_cast<int>(binaryToDecimal(b.flip())) << "\n";
    
    return 0;
}
