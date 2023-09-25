# timeR
timeR is a simple cli tool, that can launch a regular timer or a pomodoro timer. After timer 
elapses or pomodoro timer schedules a break, a sound alaram and a system notification are emitted.
## Building
```
mkdir build 
cmake ..
make
```

## Running
Start a regular timer:
```
./timeR <minutes>
```
Start a pomodoro timeR
```
./timeR <minutes_task> <minutes_break>
```

Can launch with `p` argument, to use _persistent_ mode, where it will output the state to stdout.
