#pragma once

#include "display_manager.h"
#include "human_interface.h"
#include <unordered_map>
#include <vector>

enum Display{
    NONE,
    L,
    I,
    AK,
    A,
    S
};

class W_Local
{
    private:
        DisplayManager *dispMan = nullptr;
        HumanInterface *humInter = nullptr;

        uint16_t L = 0;
        uint16_t I = 0;
        uint16_t AK = 0;
        uint16_t A = 0;
        uint16_t S = 0;
        uint16_t JAML = 0;

        std::pair<uint16_t, uint16_t> PaO[4] = {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0}};

        std::unordered_map<std::string, bool> signal = {
            {"il",    false},
            {"wel",   false},
            {"wyl",   false},
            {"wyad",  false},
            {"wei",   false},
            {"weak",  false},
            {"dod",   false},
            {"ode",   false},
            {"przep", false},
            {"wyak",  false},
            {"weja",  false},
            {"wea",   false},
            {"czyt",  false},
            {"pisz",  false},
            {"wes",   false},
            {"wys",   false}};

        
        std::unordered_map<std::string, uint16_t> bus = {
            {"A", 0},
            {"S", 0}};

        std::vector<std::string> nextLineSignals;

        using CommandFunction = void (W_Local::*)();
    
        static const std::unordered_map<std::string, CommandFunction> signalMap;
    
        Display selectedDisplay = NONE;

        /// @brief Inserts value from one element to another
        ///        capping its value to 999
        /// @param from variable's value getting inserted and capped
        /// @param to variable to insert value to
        void baseSignal(uint16_t from, uint16_t &to);
    
        void il();

        void wel();
        
        void wyl();
        
        void wyad();
        
        void wei();
        
        void weak();
        
        void dod();
        
        void ode();
        
        void przep();
        
        void wyak();
        
        void weja();
        
        void wea();
        
        void czyt();
        
        void pisz();
        
        void wes();
        
        void wys();

        void takt();
        
        void display();

        void readButtonInputs();

    public:
        W_Local(DisplayManager *dispMan, HumanInterface *humInter);
       
        ~W_Local();
        
        void runLocal();
};

