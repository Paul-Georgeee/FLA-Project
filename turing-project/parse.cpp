#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <assert.h>
#include "turingMachine.h"
using namespace std;


void parse_state_set(string s, TuringMachine &tm, int flag)
{
    //flag = 0: all states
    //flag = 1: final states
    stringstream ss(s);
    string state;
    regex e("[0-9A-Za-z_]*");
    while(getline(ss, state, ','))
    {
        if(regex_match(state.c_str(), e))
        {
            if(flag == 0)
                tm.insert_states(state);
            else if(flag == 1)
                tm.insert_final_states(state);
            else
                assert(0);
        }
        else
            throw "Wrong format of the state \"" + state + "\"";
    }
}

static inline bool check_input_symbol_valid(char c)
{
    return c != ' ' && c != ',' && c != ';' && c != '{' && c != '}' && c != '*' && c != '_';
}
void parse_input_symbol_set(string s, TuringMachine &tm)
{
    stringstream ss(s);
    string symbols;
    while (getline(ss, symbols, ','))
    {
        if(symbols.length() == 1 && check_input_symbol_valid(symbols[0]))
            tm.insert_input_symbols(symbols[0]);
        else
            throw "Wrong format of the input symbol \"" + symbols + "\"";
    }
}

static inline bool check_tape_symbol_valid(char c)
{
    return c != ' ' && c != ',' && c != ';' && c != '{' && c != '}' && c != '*';
}
void parse_tape_symbol_set(string s, TuringMachine &tm)
{
    stringstream ss(s);
    string symbols;
    while (getline(ss, symbols, ','))
    {
        if(symbols.length() == 1 && check_tape_symbol_valid(symbols[0]))
            tm.insert_tape_symbols(symbols[0]);
        else
            throw "Wrong format of the tape symbol \"" + symbols + "\"";
    }
}

static inline bool check_transform_symbol_valid(char c)
{
    return c != ' ' && c != ',' && c != ';' && c != '{' && c != '}';
}
void parse_transform_func(string old_state, string old_symbol, string new_symbol, string direct, string new_state, TuringMachine &tm)
{
    if(old_symbol.length() != new_symbol.length())
        throw "Length of the new symbol and the old symbol is not equal in transform func: \""+ new_symbol + "\", \"" + old_symbol + "\"";
    else if(old_symbol.length() != direct.length())
        throw "Length of the direct and the old symbol is not equal in transform func: \""+ direct + "\", \"" + old_symbol + "\"";
    
    for(size_t i = 0; i < old_symbol.length(); ++i)
        if(!(check_transform_symbol_valid(old_symbol[i]) && check_transform_symbol_valid(new_symbol[i])))
            throw "Wrong form of the symbol in transform func \"" + new_symbol + "\", \"" + old_symbol + "\""; 

    tm.insert_transform_func(old_state, old_symbol, new_state, new_symbol, direct);
}

regex match_annotation("[\\s]*|[\\s]*;[\\S\\s]*");
string filter_annotation(ifstream &fin)
{
    string tmp;
    while(getline(fin, tmp))
    {
        if(!regex_match(tmp.c_str(), match_annotation))
            return tmp;
    }
    return "";
}

void parse_turing(ifstream &fin, TuringMachine &tm)
{
    string tmp;
    smatch m;
    //Read the states set
    tmp = filter_annotation(fin);
    regex states_g("[\\s]*#Q = \\{(.*)\\}[\\s]*"), states_with_annotation("|[\\s]*#Q = \\{(.*)\\}[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, states_g))
        if(!regex_match(tmp, m, states_with_annotation))
            throw "Wrong gramma when defining the state set: \"" + tmp + "\"";
    assert(m.size() == 2);
    parse_state_set(m[1].str(), tm, 0);


    tmp = filter_annotation(fin);
    regex input_symbol_g("[\\s]*#S = \\{(.*)\\}[\\s]*"), input_symbol_with_annotation("[\\s]*#S = \\{(.*)\\}[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, input_symbol_g))
        if(!regex_match(tmp, m, input_symbol_with_annotation))
            throw "Wrong gramma when defining the input symbol set: \"" + tmp + "\"";
    assert(m.size() == 2);
    parse_input_symbol_set(m[1].str(), tm);


    tmp = filter_annotation(fin);
    regex tape_symbol_g("[\\s]*#G = \\{(.*)\\}[\\s]*"), tape_symbol_with_annotation("[\\s]*#G = \\{(.*)\\}[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, tape_symbol_g))
        if(!regex_match(tmp, m, tape_symbol_with_annotation))
            throw "Wrong gramma when defining the tape symbol set: \"" + tmp + "\"";
    assert(m.size() == 2);
    parse_tape_symbol_set(m[1].str(), tm);
    
    tm.check_subset();

    tmp = filter_annotation(fin);
    regex init_state_g("[\\s]*#q0 = ([a-zA-Z0-9_]*)[\\s]*"), init_state_with_annotation("[\\s]*#q0 = ([a-zA-Z0-9_]*)[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, init_state_g))
        if(!regex_match(tmp, m, init_state_with_annotation))
            throw "Wrong gramma when defining init state: \"" + tmp + "\"";
    assert(m.size() == 2);
    tm.set_init_state(m[1].str());
    

    tmp = filter_annotation(fin);
    regex blank_symbol_g("[\\s]*#B = ([\\S])[\\s]*"), blank_symbol_with_annotation("[\\s]*#B = ([\\S])[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, blank_symbol_g))
        if(!regex_match(tmp, m, blank_symbol_with_annotation))
            throw "Wrong gramma when defining the blank symbol: \"" + tmp + "\"";
    assert(m.size() == 2);
    if(m[1].str()[0] != '_')
        throw string("The blank symbol should be the \"_\"");
    tm.check_if_blank_exist();

    tmp = filter_annotation(fin);
    regex final_states_g("[\\s]*#F = \\{(.*)\\}[\\s]*"), final_states_with_annotation("[\\s]*#F = \\{(.*)\\}[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, final_states_g))
        if(!regex_match(tmp, m, final_states_with_annotation))
            throw "Wrong gramma when defining the final state set: \"" + tmp + "\"";
    assert(m.size() == 2);
    parse_state_set(m[1].str(), tm, 1);


    tmp = filter_annotation(fin);
    regex tapecnt_g("[\\s]*#N = ([1-9][0-9]*)[\\s]*"), tapecnt_with_annnotation("[\\s]*#N = ([1-9][0-9]*)[\\s]*;[\\S\\s]*");
    if(!regex_match(tmp, m, tapecnt_g))
        if(!regex_match(tmp, m, tapecnt_with_annnotation))
            throw "Wrong gramma when defining the tape cnt: \"" + tmp + "\"";
    assert(m.size() == 2);
    stringstream ss(m[1].str());
    int cnt;
    ss >> cnt;
    tm.set_tapecnt(cnt);

    regex transform_func_g("[\\s]*([0-9A-Za-z_]*) ([\\S]*) ([\\S]*) ([lr\\*]*) ([0-9A-Za-z_]*)[\\s]*");
    regex transform_func_with_annotation("[\\s]*([0-9A-Za-z_]*) ([\\S]*) ([\\S]*) ([lr\\*]*) ([0-9A-Za-z_]*)[\\s]*;[\\S\\s]*");
    while((tmp = filter_annotation(fin)) != "")
    {
        if(!regex_match(tmp, m, transform_func_g))
            if(!regex_match(tmp, m, transform_func_with_annotation))
                throw "Wrong gramma when defining the transform func: \"" + tmp + "\"";
        assert(m.size() == 6);
        parse_transform_func(m[1].str(), m[2].str(), m[3].str(), m[4].str(), m[5].str(), tm);
    }

}
