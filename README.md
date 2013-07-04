arduino-test-setup
==================

My test environnement for arduino libraries that i write.

I have created an alternative Arduino.h which mimics the functionality of the real Arduino core.
With a Makefile is selected which code is build and linked against.

I have added a DummySerial which replaces the Serial object and uses ncurses to comunicate with the user.
As soon as I need to use an arduino function which isn't part of the standard gnu libraries I add it to these 
files.

Currently I am building a MiniRPC library for the arduino and it is tested and build in this codebase.

This project builds on linuxs with ncurses.
