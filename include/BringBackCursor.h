#pragma once

#include <map>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <type_traits>
#include <sys/ioctl.h>

class BringBackCursor{
    private:
    static void sigterm_handler(int signum);
    static BringBackCursor* instance;
    public:
    BringBackCursor();
};