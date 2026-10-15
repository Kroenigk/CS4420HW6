# Homework 6

This is the code for Homework 6 of CS4420, which implements a voting system using processes and pipes in C. The program consists of a judge process that collects votes from multiple voter processes and determines the majority vote.

## Project structure

```text
HW6.c       Main program and calculation placeholder
Makefile    Build and clean commands
HW6         Generated executable
```

## Build and run

You need a C compiler and Make installed. Run these commands from this folder:

```sh
make             # Compile to HW6 in this folder
./HW6            # Run the program
make clean       # Remove the executable
```

You can also run the executable directly with `./HW6`.

## How Processes interact with each other

The judge process creates a pipe for each voter process to communication. The pipe has two ends: pipes[i]\[0] is the read end, and pipes[i]\[1] is the write end. It also forks a child process for each voter. The judge process uses read() from the pipes to get each child processes' votes and declares the majority once all votes are received. The voters use write() to the pipes with their votes randomly each round.

```mermaid
flowchart LR
    JudgeProcess(Judge Process)
    VoterProcesses1( Voter Process 1)
    VoterProcesses2( Voter Process 2)
    VoterProcesses3( Voter Process 3)
    Pipe1(Pipe 1)
    Pipe2(Pipe 2)
    Pipe3(Pipe 3)

    VoterProcesses1 -- "write()" --> Pipe1
    VoterProcesses2 -- "write()" --> Pipe2
    VoterProcesses3 -- "write()" --> Pipe3
    Pipe1 -- "read()" --> JudgeProcess
    Pipe2 -- "read()" --> JudgeProcess
    Pipe3 -- "read()" --> JudgeProcess

```

## Screenshots of the program running

![Screenshot 1](./screenshots/screenshot1.png)

![Screenshot 2](./screenshots/screenshot2.png)

![Screenshot 3](./screenshots/screenshot3.png)

![Screenshot 4](./screenshots/screenshot4.png)
