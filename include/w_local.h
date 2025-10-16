#pragma once

#include "display_manager.h"
#include "human_interface.h"
#include <unordered_map>
#include <vector>

class W_Local
{
    private:
        DisplayManager *dispMan = nullptr;
        HumanInterface *humInter = nullptr;

        std::map<std::string, uint16_t> values = {
            {"L", 0},
            {"I", 0},
            {"AK", 0},
            {"A", 0},
            {"S", 0},
            {"JAML", 0},
        };

        std::pair<uint16_t, uint16_t> PaO[4] = {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0}};

        std::unordered_map<std::string, bool> signal = {
            {"IL",    false},
            {"WEL",   false},
            {"WYL",   false},
            {"WYAD",  false},
            {"WEI",   false},
            {"WEAK",  false},
            {"DOD",   false},
            {"ODE",   false},
            {"PRZEP", false},
            {"WYAK",  false},
            {"WEJA",  false},
            {"WEA",   false},
            {"CZYT",  false},
            {"PISZ",  false},
            {"WES",   false},
            {"WYS",   false}};
 
        std::unordered_map<std::string, uint16_t> bus = {
            {"A", 0},
            {"S", 0}};

        std::vector<std::string> nextLineSignals;

        using CommandFunction = void (W_Local::*)();
    
        static const std::unordered_map<std::string, CommandFunction> signalMap;
    
        bool insertModeEnabled = false;
        std::string selectedValue = "L";

        char* lastPressedButton = nullptr;

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
        
        void refreshDisplay();

        void readButtonInputs();

        void insertMode(uint16_t &value);

        ThreeDigitDisplay *getSelectedDisplay();

        void handleInsertMode();

    public:
        W_Local(DisplayManager *dispMan, HumanInterface *humInter);
       
        ~W_Local();

        void runLocal();
};

