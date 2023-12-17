#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <iostream>
#include <ostream>
#include <sys/stat.h>
#include <unistd.h>

#include "blobber.hpp"
#include "raylib.h"
#include "ui.h"

int app(State& s);
void send_notification(std::string mode, int minutes);

int main(int argc, const char** argv)
{
	State s;
	parse(argc, argv, s);
	int minutes = 5;
	if (argc > 1)
		minutes = atoi(argv[1]);
	if (!s.persist)
	{
		auto pid = fork();
		if (pid < 0)
			std::cerr << "COULD NOT FORK! EXITING!" << std::endl;
		if (pid > 0)
			exit(0);
		close(0);
		close(1);
		close(2);
		setsid();
	}
	return app(s);
}
void play(Sound sound)
{
	PlaySound(sound);
	sleep(2);
}
void timer(int minutes)
{
	for (int i = 60 * minutes; i > 0; i -= 1)
	{
		printf("Time remaining: %d:%02d\t\t\r", i / 60, i % 60);
		sleep(1);
	}
}
void pomodoro(int worklen, int breaklen, Sound sound)
{
	bool worktime = true;
	printf("Mode: Pomodoro\n\r");
	int round = 0;
	while (1)
	{
		round++;
		printf("###########################\n\rRound %d\r\n", round);
		std::string modeString = (worktime ? "Work!" : "Rest");
		printf("Now it's time for: %s\r\n", modeString.c_str());
		send_notification(modeString, worktime ? worklen : breaklen);
		if (worktime)
			timer(worklen);
		else
			timer(breaklen);
		play(sound);
		worktime = !worktime;
	}
}
void send_notification(std::string mode, int minutes)
{
	char command[512], msg[400], args[100];
	strcpy(command, "notify-send ");
	if (mode == "timer")
		sprintf(msg, "\"Timer alarm, %d minutes elapsed!\"", minutes);
	else
		sprintf(msg, "\"Now is time for: %s\n\rTime remaining: %d:00\"", mode.c_str(), minutes);
	strcpy(args, " -t 3000");
	strcat(command, msg);
	strcat(command, args);
	if (system(command) != 0)
		perror("Could not send sys notification");
}

extern char _binary_res_blob_start;
int app(State& s)
{
	blob::BlobReader br;
	br.readMemory(&_binary_res_blob_start);

	InitAudioDevice();
	long datasize;
	void* ptr = br.getAsset("not", datasize);
	Wave w = LoadWaveFromMemory(".mp3", (const unsigned char*)ptr, datasize);
	Sound noti = LoadSoundFromWave(w);
	UnloadWave(w);

	setbuf(stdout, NULL);
	if (s.timer)
	{
		timer(s.min_work);
		play(noti);
		return 0;
	}
	if (s.pomodoro)
		pomodoro(s.min_work, s.min_break, noti);
	return 0;
}
