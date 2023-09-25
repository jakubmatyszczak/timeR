#include "ui.h"

#include <cstdlib>
#include <cstring>

bool parse(int argc, const char** argv, State& s) {
    memset(&s, 0, sizeof(s));
    if(argc <= 1)
        return false;
    for(int i = 1; i < argc; i++)
    {
        int value = atoi(argv[i]);
        if (value == 0) {
            // argument is not a number
            if(argv[i][0] == 'p')
                s.persist = true;
            continue;
        }
        if(s.min_work == 0)
            s.min_work = value;
        else
            s.min_break = value;
    }
    if(s.min_work && s.min_break)
        s.pomodoro = true;
    else
        s.timer = true;
    return true;
}
