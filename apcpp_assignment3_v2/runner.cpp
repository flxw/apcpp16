#include <string>
#include <csignal>
#include <iostream>
#include <exception>

#include <stdlib.h>
#include "externallibrary.h"

using namespace std;

void handleSIGINT() {
    static bool secondInvocation = false;

    cout << "Received SIGINT...";

    if (secondInvocation) {
        exit (0);
        cout << "skipping" << endl;
    } else {
        secondInvocation = true;
        cout << "terminating" << endl;
    }
}

void signalHandler(int signum) {
    switch(signum) {
        case SIGKILL: cout << "Received SIGKILL...terminating" << endl; exit(0); break;
        case SIGSTOP: cout << "Received SIGSTOP...terminating" << endl; exit(0); break;
        case SIGINT:  handleSIGINT(); break;
        case SIGSEGV: cout << "Received SIGSEGV...terminating" << endl; quick_exit(0); break;
        case SIGILL:  cout << "Received SIGILL...rerouting to exception" << endl; runtime_error("SIGILL recovery..."); break;
        case SIGABRT:  cout << "Received SIGABRT...terminating" << endl; quick_exit(0); break;
        case SIGFPE:  cout << "Received SIGFPE...rerouting to exception" << endl; throw runtime_error("SIGFPE recovery..."); break;
        default: break;
    }
}


void sayGoodbye() {
    cout << "So long, and thanks for all the fish." << endl;
}

int main(int argc, char * argv[])
{
    // handler setup
    atexit(sayGoodbye);
    signal(SIGINT, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGSTOP, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);

    try {
        bool dryRun = atoi(argv[1]) < 1000000;

        initialize(dryRun); //this guy ruins everything
    } catch (exception& e) {
        cout << "Caught exception: " << e.what() << " ...continuing" << endl;
    }

    while (running())
    {
        try {
            processEvents();
        } catch (exception& e) {
            cout << "Caught exception: " << e.what() << " ...continuing" << endl;
        }
    }
    

    return 0;
}
