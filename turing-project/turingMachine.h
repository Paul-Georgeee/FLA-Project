#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>

struct TuringMove{
    std::string next_state;
    std::string symbol_to_write;
    std::string direct;
    TuringMove(std::string _next_state, std::string _symbol_to_write, std::string _direct): next_state(_next_state), symbol_to_write(_symbol_to_write), direct(_direct){}
    TuringMove(){}
};

struct TapeRange{
    int start;
    int end;
    TapeRange(int _start, int _end): start(_start), end(_end){}
};

class TuringMachine
{
private:
    std::unordered_set<std::string> states;
    std::unordered_set<char> input_symbols;
    std::unordered_set<char> tape_symbols;
    std::string current_state;
    char blank_symbol = '_';
    std::unordered_set<std::string> finial_states;
    int tapecnt;
    unsigned cnt_for_print;
    std::unordered_map<std::string, std::unordered_map<std::string, TuringMove>> transform_func;
    
    std::vector<std::string> tapes;
    std::vector<int> heads; 
    std::vector<TapeRange> tapes_range;

    bool determine_nextmove(std::string now_state, std::string now_symbol, struct TuringMove &move);
    char get_header_symbol(int index);
    void change_turingID(TuringMove &move);
    void print_step(int step);
public:
    TuringMachine(){};
    void insert_states(std::string state);
    void insert_input_symbols(char symbol); 
    void insert_tape_symbols(char symbol);
    void check_subset(); //need to check if the unordered_set of input symbol is subset to the unordered_set of tape symbol
    void check_if_blank_exist();
    void set_init_state(std::string state);
    void set_tapecnt(int cnt);
    void insert_final_states(std::string state);
    void insert_transform_func(std::string old_state, std::string old_symbol, std::string new_state, std::string new_symbol, std::string direct);

    void set_input(std::string input);
    void run();
};

