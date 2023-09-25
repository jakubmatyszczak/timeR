
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <iostream>
#include <ostream>
#include <sys/stat.h>

#include "allegro5/allegro.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_audio.h"

#include "ui.h"

int app(State& s);
void send_notification(std::string mode, int minutes);

int main(int argc, const char**argv)
{
    State s;
    parse(argc, argv, s);
    int minutes = 5;
    if(argc > 1)
        minutes = atoi(argv[1]);
    if(!s.persist)
    {
        auto pid = fork();
        if(pid < 0)
            std::cerr << "COULD NOT FORK! EXITING!" << std::endl;
        if(pid > 0)
            exit(0);
        close(0);
        close(1);
        close(2);
        setsid();
    }
    std::cout << "dupa" << std::endl;
    return app(s);
}
void play(ALLEGRO_SAMPLE*sample)
{
    for(int i = 0; i < 10; i++)
    {
        al_play_sample(sample, 1.0, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
        usleep(100000);
    }
    sleep(1);
}
void timer(int minutes)
{
    for(int i = 60*minutes; i > 0; i--)
    {
        printf("Time remaining: %d:%d\t\t\r", i / 60, i % 60);
        sleep(1);
    }
}
void pomodoro(int worklen, int breaklen, ALLEGRO_SAMPLE*sample)
{
    bool worktime = true;
    printf("Mode: Pomodoro\n\r");
    int round = 0;
    while(1)
    {
        round++;
        printf("###########################\n\rRound %d\r\n", round);
        std::string modeString =(worktime ? "Work!" : "Rest");
        printf("Now it's time for: %s\r\n", modeString.c_str());
        send_notification(modeString, worktime ? worklen : breaklen);
        if(worktime)
        {
            timer(worklen);
            play(sample);
        }
        else
        {
            timer(breaklen);
            play(sample);
        }
        worktime = !worktime;
    }
}
void send_notification(std::string mode, int minutes)
{
    char command[512], msg[400], args[100];
    strcpy(command,"notify-send ");
    if(mode == "timer")
        sprintf(msg, "\"Timer alarm, %d minutes elapsed!\"", minutes);
    else
        sprintf(msg, "\"Now is time for: %s\n\rTime remaining: %d:00\"", mode.c_str(), minutes);
    strcpy(args, " -t 3000");
    strcat(command,msg);
    strcat(command,args);
    if(system(command) != 0)
        perror("Could not send sys notification");
}
int app(State& s)
{
    al_init();
    al_install_audio();
    al_init_acodec_addon();
    
    ALLEGRO_SAMPLE*pew = al_load_sample("res/pew.wav");
    al_reserve_samples(20);
    setbuf(stdout, NULL);
    if(s.timer)
    {
        timer(s.min_work);
        play(pew);
        return 0;
    }
    if(s.pomodoro)
        pomodoro(s.min_work, s.min_break, pew);
    return 0;
}
