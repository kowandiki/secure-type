# Secure Type

Secure Type aims to send strings in such a way that it is very difficult for a keylogger to keep track of what is sent.
For a list of features/methods it supports check features below.

## setup

Currently, you'll need to alter main.cpp to have it send what you want.
I plan on making it take command line arguments, with the phrase it is going to type out as one of those args.
Because of this, it is highly recommended you integrate this into a password manager.

## features
- break a larger string into several smaller ones
- send string in a randomized order
- should work on any keyboard assuming it uses only ascii (+ extended) characters
- send extra/garbage characters
- use the clipboard
- switch focus on and off of target application so that it appears to send characters
- global hotkey support (further support coming with command line args)

## planned
- use ctrl to help navigate
- command line args
- unicode support
- swap keyboard / languages
- linux version
- mac version
