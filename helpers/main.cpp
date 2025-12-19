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

template <size_t N>
std::bitset<N> uint8_tTo_binary(uint8_t number)
{
    std::bitset<N> result;
    int index = 0;
    while (number > 0){
        int bit = number%2;
        result[index++] = bit;
        number /= 2;
    }

    return result;
}

template <size_t N>
void printBitset(const std::bitset<N>& bits)
{
    for (int i = N - 1; i >= 0; i--) {
        std::cout << bits[i];
    }
}

int main(){
    std::bitset<8> a("11111111");
    std::bitset<8> b("11111111");

    std::cout << "Wynik: ";
    printBitset(uint8_tTo_binary<8>(uint8_t(300)));
    // std::cout << "Wynik: " << static_cast<int>(binaryToDecimal(b.flip())) << "\n";
    
    return 0;
}
