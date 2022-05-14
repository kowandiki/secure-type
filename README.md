# Secure Type

Secure Type aims to send strings in such a way that it is very difficult for a keylogger to keep track of what is sent.
For a list of features/methods it supports check features below.

## setup / how to use

run the executable with the following parameters:
1) word chunk size (int)
2) what program to use (int) (this doesn't work right now)
3) size multiplier (float, must be greater than 1)
4) focus swap multiplier (float, must be postive)
5) delay between arrow keys (int, in ms)
6) delay between characters (int, in ms)
7) delay between chunks (int, in ms)
8) send garbage (bool, "t"/"f")
9) use clipboard (bool, "t"/"f")
10) swap focus (bool, "t"/"f")
11) use ctrl (bool, "t"/"f") (this doesn't work right now)
12) vk of hotkey (int) (for list see below)
13) modifier keys (int)
14) word to send, all future arguments will be sent<br><br>
[vk list](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)<br>
modifier keys:
- ctrl = 2
- alt = 1
- shift = 4
- windows key = 8
- no repeat = 16384
just add the ones you want together and pass that in

## features
- break a larger string into several smaller ones
- send string in a randomized order
- should work on any keyboard assuming it uses only ascii (+ extended) characters
- send extra/garbage characters
- use the clipboard
- switch focus on and off of target application so that it appears to send characters
- global hotkey support
- command line args (There is no parser so be sure to send everything correctly)

## planned
- use ctrl to help navigate
- unicode support
- swap keyboard / languages
- linux version
- mac version
