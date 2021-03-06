#pragma once

#ifndef insertion_h
#define insertion_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
using namespace std;
using namespace std::chrono;

class Insertion {
    public:
        map<string, long> insertion(string, string, int, int);
        map<string, long> insertion_recur(string, string, int, int);
        map<string, long> insertion_modular(string, string, int, int);
};

#endif 