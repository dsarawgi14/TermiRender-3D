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

#include "vec.h"
#include "vecFunctions.h"
#include "objectClass.h"
#include "renderableObject.h"
#include "BringBackCursor.h"

void BringBackCursor::sigterm_handler(int signum) {
    if (signum == SIGINT || signum == SIGTSTP) {
        printf("\e[?25h");
        exit(0);  // You may want to perform additional cleanup or handling before exiting
    }
}
BringBackCursor::BringBackCursor() {
    if(BringBackCursor::instance == nullptr) {
        BringBackCursor::instance = this;
        signal(SIGINT, sigterm_handler);
        signal(SIGTSTP, sigterm_handler);
    }
}
BringBackCursor* BringBackCursor::instance = nullptr;