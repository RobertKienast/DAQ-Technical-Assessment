
#include "decoder.hpp"
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>

// So essentially all the functionality is done in decoder
// due to the testing of catch2 - testing with a main function is a pain

// I ain't gon lie I did not read that it had to be written to file not stdout
// so this a makeshift solution


int main()
{

    decode("../../dbc-files/ControlBus.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc", "../../dump.log");
    return 0;
}
