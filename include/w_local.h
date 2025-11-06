#pragma once

#include "display_manager.h"
#include "human_interface.h"
#include <unordered_map>
#include <vector>

/**
 * @file w_local.h
 * @brief Local machine control mode without network connectivity
 * 
 * Implements the local operation mode where the machine is controlled via physical
 * buttons, rotary encoder, and serial input. Manages machine state including registers,
 * signals, memory, and real-time display updates.
 * 
 * **Features:**
 * @li Register manipulation (L, I, AK, A, S, JAML)
 * @li Signal execution with conflict detection
 * @li Program-Address-Operand (PAO) memory display
 * @li Insert mode for value editing via rotary encoder
 * @li Serial debug interface
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */

class W_Local
{
private:
    DisplayManager *dispMan = nullptr;                       ///< Pointer to display manager for hardware control
    HumanInterface *humInter = nullptr;                      ///< Pointer to human interface for input handling

    /// Machine register values map
    /// @li L - Counter/Instruction pointer
    /// @li I - Instruction register
    /// @li AK - Accumulator
    /// @li A - Address register
    /// @li S - Stack pointer
    /// @li JAML - Working register
    std::map<std::string, uint16_t> values = {
        {"L", 0},
        {"I", 0},
        {"AK", 0},
        {"A", 0},
        {"S", 0},
        {"JAML", 0},
    };

    /// Program-Address-Operand memory (64 entries)
    /// Each entry contains a pair of (address, operand) values
    /// Stores the instruction set for the machine
    std::pair<uint16_t, uint16_t> PaO[64] = {
        {1, 1},   {2, 2},   {4, 4},   {8, 8},   {17, 17}, {34, 34}, {36, 36}, {56, 56},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},
        {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0},   {0, 0}
    };

    /// Signal state map (on/off state for each signal line)
    /// Contains all 16 control signals used by the machine
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
        {"WYS",   false}    
    };

    /// Data bus values (A and S buses)
    std::unordered_map<std::string, uint16_t> bus = {
        {"A", 0},           ///< Address bus value
        {"S", 0}            ///< Stack bus value
    };

    /// Queue of signals to execute on next TAKT
    /// Stores selected signals before execution
    std::vector<std::string> nextLineSignals;

    /// Function pointer type for signal command methods
    using CommandFunction = void (W_Local::*)();

    /// Map of signal names to their corresponding command methods
    /// Used for dynamic signal execution
    static const std::unordered_map<std::string, CommandFunction> signalMap;

    /// Map of signal conflicts (signals that cannot be active simultaneously)
    /// Maps signal name to list of conflicting signals
    static const std::unordered_map<std::string, std::vector<std::string>> signalConflicts;

    bool insertModeEnabled = false;                          ///< Flag indicating if insert/edit mode is active
    std::string selectedValue = "L";                         ///< Currently selected register in insert mode

    char* lastPressedButton = nullptr;                       ///< Tracks last button pressed for debouncing

    /**
     * @brief Clamp and transfer a value between two variables
     * 
     * Transfers a value from source to destination, clamping it to the valid range (0-999).
     * Used as a common utility for signal operations that move data between registers and buses.
     * 
     * @param from Source value to transfer
     * @param to Destination reference to receive the clamped value
     * 
     * @note Value is clamped: values > 999 become 999, values < 0 become 0
     */
    void baseSignal(uint16_t from, uint16_t &to);

    /// @name Signal Command Methods
    /// These methods implement the actual machine operations when signals are executed
    /// @{

    /** @brief IL (Instruction Load) - Increment the L (counter) register */
    void il();

    /** @brief WEL (Write Enable Line) - Transfer value from bus A to register L */
    void wel();

    /** @brief WYL (Write Output Line) - Transfer value from register L to bus A */
    void wyl();

    /** @brief WYAD (Write Address) - Transfer value from register I to bus A */
    void wyad();

    /** @brief WEI (Write Enable Input) - Transfer value from bus S to register I */
    void wei();

    /** @brief WEAK (Write Enable AK) - Transfer value from register JAML to register AK */
    void weak();

    /** @brief DOD (Increment) - Add AK to JAML and store result */
    void dod();

    /** @brief ODE (Decrement) - Subtract AK from JAML and store result */
    void ode();

    /** @brief PRZEP (Transfer) - Transfer value from register JAML to register AK */
    void przep();

    /** @brief WYAK (Write Output AK) - Transfer value from register AK to bus S */
    void wyak();

    /** @brief WEJA (Write Enable JA) - Transfer value from bus S to register JAML */
    void weja();

    /** @brief WEA (Write Enable A) - Transfer value from bus A to register A (with range check 0-63) */
    void wea();

    /** @brief CZYT (Read) - Read from PAO memory at address A into register S */
    void czyt();

    /** @brief PISZ (Write) - Write to PAO memory at address A from register S */
    void pisz();

    /** @brief WES (Write Enable Stack) - Transfer value from register S to bus S */
    void wes();

    /** @brief WYS (Write Output Stack) - Transfer value from register S to bus S */
    void wys();

    /// @}

    /**
     * @brief Execute all queued signals in sequence
     * 
     * Executes each signal in the nextLineSignals queue by calling their corresponding
     * command methods. After execution, clears all signal states and the queue.
     * 
     * Only executes if the queue is not empty.
     */
    void takt();

    /**
     * @brief Update all displays with current machine state
     * 
     * Refreshes all connected displays (three-digit displays, signal lines, bus lines, PAO displays)
     * with the current machine state values. Calls dispMan->refreshDisplay() at the end.
     * 
     * @note Called every loop iteration to keep display synchronized
     */
    void refreshDisplay();

    /**
     * @brief Read and process button/encoder input from hardware
     * 
     * Polls the human interface for button presses and manages the signal queue:
     * @li TAKT button → Executes queued signals immediately
     * @li Signal button → Toggles signal in queue (add if not present, remove if present)
     * @li Implements debouncing via lastPressedButton tracking
     * 
     * @note Called every loop iteration
     */
    void readButtonInputs();

    /**
     * @brief Handle value modification in insert mode via rotary encoder
     * 
     * Allows the user to increment/decrement the currently selected register value
     * using the rotary encoder. Values wrap around (0 ↔ 999).
     * 
     * @param value Reference to the register value to modify
     * 
     * **Encoder behavior:**
     * @li UP rotation → Increment value (wrap 999→0)
     * @li DOWN rotation → Decrement value (wrap 0→999)
     */
    void insertMode(uint16_t &value);

    /**
     * @brief Get the display pointer for the currently selected register
     * 
     * Returns the corresponding ThreeDigitDisplay for the register selected in insert mode.
     * Used to highlight and animate the active register during editing.
     * 
     * @return Pointer to ThreeDigitDisplay for the selected register (A, AK, L, I, S),
     *         or nullptr if no valid display exists
     * 
     * @see selectedValue
     */
    ThreeDigitDisplay* getSelectedDisplay();

    /**
     * @brief Handle insert/edit mode activation and register selection
     * 
     * Manages the insert mode interface:
     * @li Long-press encoder button → Toggle insert mode on/off
     * @li Short-press encoder button → Select next register (cycles through all registers)
     * @li Encoder rotation → Modify selected register value
     * @li Blinking animation → Indicates currently selected register
     * 
     * @note Called every loop iteration when not in WiFi mode
     */
    void handleInsertMode();

    /**
     * @brief Process serial debug commands from UART
     * 
     * Reads commands from serial console and executes them as if they were button presses.
     * Supports all signal names and "TAKT" command.
     * 
     * **Example commands:**
     * @li "IL" → Add/toggle IL signal
     * @li "TAKT" → Execute queued signals
     * @li Unknown commands print list of valid commands to serial
     * 
     * @note Useful for testing without physical hardware
     */
    void handleSerialDebug();

    /**
     * @brief Print current machine state to serial console
     * 
     * Outputs a formatted state report including:
     * @li Queued signals (nextLineSignals)
     * @li Register values (L, I, AK, A, S, JAML)
     * @li PAO memory contents (first 4 entries)
     * 
     * Prints at regular intervals (500ms) to avoid console spam.
     */
    void printValuesToSerial();

    /**
     * @brief Validate that a signal can be added without conflicts
     * 
     * Checks if the new signal conflicts with any currently active signals
     * in the nextLineSignals queue using the signalConflicts map.
     * 
     * **Conflict examples:**
     * @li CZYT ↔ PISZ (cannot read and write simultaneously)
     * @li DOD ↔ ODE ↔ PRZEP (cannot increment, decrement, and transfer simultaneously)
     * 
     * @param newSignal Name of the signal to validate
     * 
     * @return true if signal can be added (no conflicts), false otherwise
     * 
     * @note Prints debug message to serial on conflict detection
     * @see signalConflicts
     */
    bool isSignalValid(const std::string &newSignal);

public:
    /**
     * @brief Construct a new W_Local machine controller
     * 
     * Initializes the local machine with references to the display manager
     * and human interface. Machine state is initialized with all registers at 0
     * and all signals deactivated.
     * 
     * @param dispMan Pointer to DisplayManager for display control. Must not be nullptr.
     * @param humInter Pointer to HumanInterface for input handling. Must not be nullptr.
     */
    W_Local(DisplayManager *dispMan, HumanInterface *humInter);

    /**
     * @brief Destructor
     * 
     * Cleans up resources (currently no dynamic allocation in this class).
     */
    ~W_Local();

    /**
     * @brief Main loop for local machine operation
     * 
     * Executes the local machine control sequence in order:
     * @li handleSerialDebug() - Process serial commands
     * @li handleInsertMode() - Handle register editing
     * @li readButtonInputs() - Read button/encoder input
     * @li refreshDisplay() - Update all displays
     * @li printValuesToSerial() - Output state to serial
     * 
     * @note Should be called continuously from main loop when in local mode
     */
    void runLocal();
};