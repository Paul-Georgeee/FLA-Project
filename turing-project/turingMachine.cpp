#include <iostream>
#include <assert.h>
#include <sstream>
#include "turingMachine.h"

using namespace std;

extern bool verbose_mode;

void TuringMachine::insert_states(std::string state)
{
    //need to check if the state has exist
    if(this->states.find(state) == this->states.end())
        this->states.insert(state);
    else
        throw "State: " + state + " has existed";
}
void TuringMachine::insert_input_symbols(char symbol)
{
    if(this->input_symbols.find(symbol) == this->input_symbols.end())
        this->input_symbols.insert(symbol);
    else
        throw "Input symbol: " + string(1, symbol) + " has exisited";
}
void TuringMachine::insert_tape_symbols(char symbol)
{
    if(this->tape_symbols.find(symbol) == this->input_symbols.end())
        this->tape_symbols.insert(symbol);
    else
        throw "Tape symbol: " + string(1, symbol) + " has exisited";
}
void TuringMachine::check_subset()
{
    for(auto it = this->input_symbols.begin(); it != this->input_symbols.end(); ++it)
    {
        if(this->tape_symbols.find(*it) == this->tape_symbols.end())
            throw "The input symbol set must be subset to the tap symbol set where \"" + string(1, *it) + "\" is not in tape symbol";
    }
}
void TuringMachine::set_init_state(std::string state)
{
    if(this->states.find(state) == this->states.end())
        throw "The init state \"" + state + "\" is not in the state set";
    this->current_state = state;
}

static inline unsigned count(int a)
{
    if(a == 0)
        return 1;
    unsigned res = 0;
    do
    {
        res += 1;
        a /= 10;
    } while (a > 0);
    return res;
}
void TuringMachine::set_tapecnt(int cnt)
{
    if(cnt <= 0)
        throw string("Tape cnt should greater than 0");
    this->tapecnt = cnt;
    this->cnt_for_print = count(cnt - 1) + 6;
}
void TuringMachine::insert_final_states(std::string state)
{
    if(this->states.find(state) == this->states.end())
        throw "The final state \"" + state + "\" is not in the state set";
    else if(this->finial_states.find(state) != this->states.end())
        throw "The final state \"" + state + "\" has been in the final state set";
    else
        this->finial_states.insert(state);
}
void TuringMachine::insert_transform_func(std::string old_state, std::string old_symbol, std::string new_state, std::string new_symbol, std::string direct)
{
    //cout << "insert transform func:" << old_state << " " << old_symbol << " " << new_symbol << " " << new_symbol << " " << direct << endl;
    if(this->states.find(old_state) == this->states.end())
        throw "The old state \"" + old_state + "\" in transform func is not in state set";
    if(this->states.find(new_state) == this->states.end())
        throw "The new state \"" + new_state + "\" in transform func is not in state set";
    assert(old_symbol.length() == new_symbol.length());
    if(old_symbol.length() != (unsigned)this->tapecnt)
        throw "The length of old symbol \"" + old_symbol + "\" does not match the tape count";
    
    for(size_t i = 0; i < old_symbol.size(); ++i)
    {
        if(old_symbol[i] != '*' && this->tape_symbols.find(old_symbol[i]) == this->tape_symbols.end())
            throw "The symbol \"" + string(1, old_symbol[i]) + "\" in transform function is not existed in tape symbol set";
        if(new_symbol[i] != '*' && this->tape_symbols.find(new_symbol[i]) == this->tape_symbols.end())
            throw "The symbol \"" + string(1, new_symbol[i]) + "\" in transform function is not existed in tape symbol set";
        if(old_symbol[i] != '*' && new_symbol[i] == '*')
            throw string("Can not use * in new symbol when the old symbol is *");
    }

    auto it = this->transform_func.find(old_state);
    TuringMove move(new_state, new_symbol, direct);
    if(it != this->transform_func.end())
    {
        if(it->second.find(old_symbol) == it->second.end())
            it->second.insert(make_pair(old_symbol, move));
    }
    else
    {
        unordered_map<string, TuringMove> m;
        m.insert(make_pair(old_symbol, move));
        this->transform_func.insert(make_pair(old_state, m));
    }  
    
}

void TuringMachine::check_if_blank_exist()
{
    if(this->tape_symbols.find('_') == this->tape_symbols.end())
        throw string("The blank symbol _ is not existed");
}

void TuringMachine::set_input(std::string input)
{
    for(size_t i = 0; i < input.length(); ++i)
        if(this->input_symbols.find(input[i]) == this->input_symbols.end())
            throw "Input: " + input 
            + "\n==================== ERR ====================\nerror: \'" 
            + input[i]+ "\' was not declared in the set of input symbols\nInput: " 
            + input + "\n"
            + string(7 + i, ' ') 
            + "^\n==================== END ====================";
    this->tapes.push_back(input);
    this->heads.push_back(0);
    this->tapes_range.push_back(TapeRange(0, input.length()));
    for(int i = 1; i < this->tapecnt; ++i){
        this->tapes.push_back("");
        this->heads.push_back(0);
        this->tapes_range.push_back(TapeRange(0, 0));
    }
    if(verbose_mode)
        cout << "Input: " << input << endl << "==================== RUN ====================" << endl;
}

static bool compare(string now_symbol, string transform)
{
    assert(now_symbol.length() == transform.length());
    for(size_t i = 0; i < now_symbol.length(); ++i)
    {
        if((transform[i] == '*' && now_symbol[i] != '_') || now_symbol[i] == transform[i])
            continue;
        else
            return false;
    }
    return true;
}
bool TuringMachine::determine_nextmove(std::string now_state, std::string now_symbol, struct TuringMove &move)
{
    auto it = this->transform_func.find(now_state);
    if(it == this->transform_func.end())
        return false;
    
    unordered_map<string, TuringMove> &val = it->second;
    auto tmp = val.find(now_symbol);
    if(tmp != val.end())
    {
        move.direct = tmp->second.direct;
        move.next_state = tmp->second.next_state;
        move.symbol_to_write = tmp->second.symbol_to_write;
        return true;
    }
    else
    {
        for(auto i = val.begin(); i != val.end(); ++i)
        {
            if(compare(now_symbol, i->first))
            {
                move.direct = i->second.direct;
                move.next_state = i->second.next_state;
                move.symbol_to_write = i->second.symbol_to_write;
                return true;
            }
        }
    }
    return false;

}

static inline bool check_in_range(int start, int end, int index){
    return index >= start && index < end;
}
char TuringMachine::get_header_symbol(int index)
{
    int head = this->heads[index];
    if(check_in_range(this->tapes_range[index].start, this->tapes_range[index].end, head))
        return this->tapes[index][head - tapes_range[index].start];
    else
        return '_';
}

void TuringMachine::change_turingID(TuringMove &move)
{
    this->current_state = move.next_state;
    for(int i = 0; i < tapecnt; ++i)
    {
        int head = this->heads[i];
        TapeRange &r = this->tapes_range[i];
        char old_symbol = this->get_header_symbol(i);
        char new_symbol = move.symbol_to_write[i] == '*' ? old_symbol : move.symbol_to_write[i];
        if(check_in_range(r.start, r.end, head))
            this->tapes[i][head - r.start] = new_symbol;
        else if(head == r.start - 1)
        {
            r.start = r.start - 1;
            this->tapes[i].insert(0, 1, new_symbol);
        }
        else if(head == r.end)
        {
            r.end = r.end + 1;
            this->tapes[i].push_back(new_symbol);
        }
        
        switch (move.direct[i])
        {
        case 'l':
            this->heads[i] = head - 1;
            break;
        case 'r':
            this->heads[i] = head + 1;
            break;
        case '*':
            break;
        default:
            assert(0);
        }
    }
}


void TuringMachine::print_step(int step){
    string line1 = "Step" + string(this->cnt_for_print - 4, ' ') + ": ";
    cout<< line1 << step << endl;

    string line2 = "State" + string(this->cnt_for_print - 5, ' ') + ": " + this->current_state;
    cout << line2 << endl;

    for(int i = 0; i < this->tapecnt; ++i)
    {
        
        string tmp = to_string(i);
        unsigned len = tmp.length();
        string line3 = "Index" + tmp + string(this->cnt_for_print - 5 - len, ' ') + ":";
        string line4 = "Tape" + tmp + string(this->cnt_for_print - 4 - len, ' ') + ":";
        string line5 = "Head" + tmp + string(this->cnt_for_print - 4 - len, ' ') + ":";
        
        TapeRange &r = this->tapes_range[i];
        int head = this->heads[i];
        string &tape = this->tapes[i];
        int print_start = r.start, print_end = r.end - 1;
        assert(tape.length() == (r.end - r.start));

        for(int i = 0; i < tape.length(); ++i)
            if(tape[i] == '_')
                print_start++;
            else
                break;

        for(int i = tape.length() - 1; i > (print_start - r.start); --i)
            if(tape[i] == '_')
                print_end--;
            else
                break;
        
        if(print_start <= print_end)
        {
            print_start = head >= print_start ? print_start : head;
            print_end = head <= print_end ? print_end : head;
        }
        else //empty tape
            print_start = print_end = head;
    
        for(int j = print_start; j <= print_end; ++j)
        {
            int index = j >= 0 ? j : -j;
            unsigned len = count(index);

            line3 += " " + to_string(index);
            if(check_in_range(r.start, r.end, j)) 
                line4 += " " + string(1, tape[j - r.start]) + string(len - 1, ' ');
            else //head may out of range, eg: head = r.end or head = r.start - 1
                line4 += " _" + string(len - 1, ' ');
            
            if(head == j)
                line5 += " ^" + string(len - 1, ' ');
            else
                line5 += string(len + 1, ' ');
        }

        cout << line3 << endl << line4 << endl << line5 << endl;
             
    }
    cout << "---------------------------------------------" << endl;
}
static void print_result(string &res)
{
    int i = 0, j = res.length() - 1;
    for(; i < res.length(); ++i)
        if(res[i] != '_')
            break;

    for(; j > i; --j)
        if(res[j] != '_')
            break;

    for(; i <= j; ++i)
        cout << res[i];
    cout << endl;
}

void TuringMachine::run()
{
    int step = 0;
    TuringMove move;
    while (1)
    {
        if(verbose_mode)
            print_step(step);

        string now_symbol;
        for(int i = 0; i < tapecnt; ++i)
            now_symbol.push_back(this->get_header_symbol(i));
        if(this->determine_nextmove(this->current_state, now_symbol, move) == false)
            break;
        this->change_turingID(move);
        step ++;
    }

    if(verbose_mode)
    {
        cout << "Result: ";
        print_result(tapes[0]);
        cout << "==================== END ====================" << endl;
    }
        //cout << "Result: " << tapes[0] << endl << "==================== END ====================" << endl;
    else
        print_result(tapes[0]);
}