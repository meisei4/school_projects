# CS 224 Assignment 6


## IRC Bot

IRC stands for Internet Relay Chat. It is a client/server protocol that allows remotely distributed users to communicate through text based "chat room"./ It was first defined in RFC 1459 and updated in 2000 in RFC 2812. 

Libircclient is a C API for writing client programs that connect to IRC servers. Libircclient is event based. Client program define callback functions to handle specific events such as connecting to the server, joining a chat room and receiving messages. The program then connects to an IRC server and starts the event looping running. 

The sample program defines the three essential callbacks. It responds to the any message containing its nickname with a default response. It also responds to the "say hello" message by saying hello to the sending. 

Extend the sample bot to do new things. Such as

* respond to "time" with the current local time
* respond to "set alarm" by setting an alarm
* respond to "remember"/"remind"/"forget" by remembering and reminding the sender with the requested messages
* use expreval.c to evaluate expressions
* invoke curl requests to Mashape APIs and reply with the response (e.g. current weather conditions)
* play Bingo! with the BingoBot, this requires
    * responding to "Starting new game" with "BingoBot I'm in"
    * responding to "(nickname) your number is xxx" by remembering your number
    * responding to "call xxx" by testing whether the called number is a factor of your number
    * if you get all the factors of your number your yell "BingoBot BINGO!"
* create a Tic Tac Toe playing bot
* anything else you can think of

It should also respond to "help" by listing the commands that it supports. 

The IRC server is located at 138.49.184.133 on port 6667 and the chat room for this assignment will be "cs224".
 
Documentation for libircclient can be found at <http://libircclient.sourceforge.net>.
 
## Getting and Building libircclient
 
Once you have forked and cloned this respository, "cd" into the respository and execute ``git clone https://github.com/shaoner/libircclient.git``. This will give you a libircclient directory. "cd" into libircclient and execute ``./configure`` followed by ``make``. This will build the libircclient library. It should build on both OS X and Linux. 

If you are using the Windows Subsystem for Linux you may need to also install the C++ and curl libraries with

```
sudo apt-get install g++ libcurl4-openssl-dev
```


 
## What To Do

- Fork this repository in GitLab
- Clone your fork to your development machine.
- Implement the assignment. 
- "make clean" and then "git add/commit/push" back to GitLab.
- Add me as a reporter member of your repository.
- Create an issue with me as the assignee. Use the issue title "Assign5 Ready for Grading"."
- In the issue make a note of your difficulty factor estimate for this assignment relative to the first assignment. 

