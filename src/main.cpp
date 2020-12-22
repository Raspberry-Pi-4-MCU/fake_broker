#include "broker.hpp"

int main(int argc, char** argv){
    schedule new_schedule;
    new_schedule.start();
    while(true){
        sleep(1);
    }
    return 0;
}