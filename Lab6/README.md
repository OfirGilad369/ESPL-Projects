# Lab 6

## Lab Goals
* Get acquainted with command interpreters ("shell").
* Understand how Unix/Linux fork() works.
* Learn how to read the manual (man).
* Write tedious code in C, and address non-trivial 
* memory allocation/deallocation issues.
* Learn to debug your code!

## Motivation
Perhaps the most important system program is the command interpreter, that is, the program that gets user commands and executes them. The command interpreter is thus the major interface between the user and the operating system services. There are two main types of command interpreters:
* Command-line interpreters, which receive user commands in text form and execute them (also called shell in UNIX-like systems).
* Menu-based interpreters, where the user selects commands from a menu. At the most basic level, menus are text driven. At the most extreme end, everything is wrapped in a nifty graphical display (e.g. Windows or KDE command interpreters).

## Lab Goals
In this sequence of labs, you will be implementing a simple shell (command-line interpreter). Like traditional UNIX shells, your shell program will also be a user level process (just like all your programs to-date), that will rely heavily on the operating system's services. Your shell should do the following:
* Receive commands from the user.
* Interpret the commands, and use the operating system to help starting up programs and processes requested by the user.
* Manage process execution (e.g. run processes in the background, kill them, etc.), using the operating system's services.

The complicated tasks of actually starting up the processes, mapping their memory, files, etc. are strictly a responsibility of the operating system, and as such you will study these issues in the Operating Systems course. Your responsibility, therefore, is limited to telling the operating system which processes to run, how to run these processes (run in the background/foreground) etc.

Starting and maintaining a process involves many technicalities, and like any other command interpreter we will get assistance from system calls, such as execv, fork, waitpid (see man on how to use these system calls).
