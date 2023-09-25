#pragma once

struct State 
{
    bool timer;
    bool pomodoro;
    int min_work;
    int min_break;
    bool persist;
};
bool parse(int argc, const char** argv, State& s);

