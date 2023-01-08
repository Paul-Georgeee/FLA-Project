#include <iostream>
#include <fstream>
#include <string>
#include "turingMachine.h"
#include "parse.h"
using namespace std;

bool verbose_mode = false;
static string helpstr = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
static string opts[] = {"-v", "--verbose", "-h", "--help"};
string turing_input = "";

void matchopts(char *arg)
{
    if(opts[0] == arg || opts[1] == arg)
        verbose_mode = true;
    else if(opts[2] == arg || opts[3] == arg)
    {
        cout << helpstr << endl;
        exit(0);
    }
    else
    {
        cerr << "Unrecognized arguement " << arg << endl;
        exit(-1);
    }
}

string handle_args(int argc, char *argv[])
{
    if(argc == 1)
    {
        cout << helpstr << endl;
        exit(-1);
    }

    int i = 1;
    for(; i < argc; ++i)
    {
        if(argv[i][0] == '-')
            matchopts(argv[i]);
        else
            break;
    }
    
    if(i >= argc){
        cerr << "Missing Input File" << endl;
        cerr << helpstr << endl;
        exit(-1);
    }
    string filename = argv[i];
    
    if(i + 1 < argc)
        turing_input = argv[i + 1];
    
    return filename;
}

int main(int argc, char* argv[]){

    string filename = handle_args(argc, argv);    
    ifstream fin;
    fin.open(filename, ios::in);

    if(!fin){
        cerr << "File " << filename << " doesn't exist\n";
        return -1;
    }
    TuringMachine tm;
    try{
        parse_turing(fin, tm);
    }catch(string message){
        if(verbose_mode)
            cerr << message << endl;
        else
            cerr << "syntax error" <<endl;
        return -1;
    }
    try{
        tm.set_input(turing_input);
    }catch(string message){
        if(verbose_mode)
            cerr << message << endl;
        else
            cerr << "illegal input" <<endl;
        return -1;
    }
    try{
        tm.run();
    }catch(string messages){
        cerr << messages << endl;
    }
    return 0;
}