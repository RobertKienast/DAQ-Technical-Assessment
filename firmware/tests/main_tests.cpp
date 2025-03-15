#include <catch2/catch_all.hpp>
#include "../solution/decoder.cpp"
#include <sstream>
#include <stdio.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>



#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

// Since I though the output was stdout I make a pipe to
// capture the output back into a vector ... lol
void pipeInput(std::string can0, std::string can1, std::string can2, std::string dumpLog, std::vector<std::string> *lines) {
    int pipefd[2];
    pipe2(pipefd, O_CLOEXEC);
    pid_t pid = fork();
    char buffer[128];

    if (pid == 0) {
        close(pipefd[0]);
        int stdout_backup = dup(STDOUT_FILENO);
        FILE *f = fdopen(pipefd[1], "w");
        dup2(pipefd[1], STDOUT_FILENO);
        
        // DO NOT FUCKING TOUCH THIS WILL BREAK EVERYTHING IF YOU REMOVE
        // IT... I DON"T KNOW WHY BUT FUCKING DON'T
        std::cout << std::endl;

        close(pipefd[1]);
        decode(can0, can1, can2, dumpLog);

        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
        fclose(f);
    } else {
        close(pipefd[1]);
        
        FILE *f = fdopen(pipefd[0], "r");
        while (fgets(buffer, 127, f) != NULL) {
            lines->push_back(buffer);
        }

        fclose(f);
        close(pipefd[0]);
        wait(NULL);
    }
}


TEST_CASE("General does the given input work", "[general - known]") {
    std::vector<std::string> *lines = new std::vector<std::string>;
    pipeInput("../../dbc-files/ControlBus.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc",
            "../test_dump/dump1.log", lines);
    std::vector<std::string> linesValid = {};
    std::string buff;
    if ((*lines).size() > 1) {
        for (size_t i{0}; i < lines->size(); i++) {
        
            lines->at(i).erase(std::remove(lines->at(i).begin(), lines->at(i).end(), '\n'), lines->at(i).cend());
            //linesValid.push_back((*lines)[i]);
        }
        lines->erase(lines->begin());
        
    
        
        std::vector<std::string> expectedOutputs = {"(1730892639.316946): CoolantPressureFanOUT: -1724.5",
                                                    "(1730892639.316946): CoolantPressureFanIN: -3276.8",
                                                    "(1730892639.316946): CoolantInverterPressure: 3276.7",
                                                    "(1730892639.317588): Pack_Current: 0",
                                                    "(1730892639.317588): Pack_Inst_Voltage: 485.3",
                                                    "(1730892639.317588): Pack_SOC: 14.5",
                                                    "(1730892639.317588): Relay_State: 30793",
                                                    "(1730892639.317234): Speed_actual: 0",
                                                    "(1730892639.317234): Speed_target: 0",
                                                    "(1730892639.317234): Steering_angle_actual: -1.5",
                                                    "(1730892639.317234): Steering_angle_target: 0",
                                                    "(1730892639.317234): Brake_hydr_actual: 255",
                                                    "(1730892639.317234): Brake_hydr_target: 0",
                                                    "(1730892639.317234): Motor_moment_actual: 0",
                                                    "(1730892639.317234): Motor_moment_target: 0"};
    
        REQUIRE(expectedOutputs == (*lines));      
    }
    delete lines;                         
}

TEST_CASE("Does the can frame get correctly parsed", "[general - unknown]") {
    struct canframe *head = new struct canframe;
    extractDumpLog(head, "../test_dump/dump2.log");
    struct canframe *current = head;
    std::string expectedInterfaces[3] = {"can0", "otherInterface", "A_test"};
    uint64_t expectedTimeSeconds[3] = {2639, 2739, 55839};
    uint64_t expectedTimeFracSeconds[3] = {316999, 999, 616999};
    uint64_t expectedCAN_ID[3] = {1301, 2730, 273};
    uint64_t expectedData[3] = {1311768467463790321, 0, 12297641732500815872};

    int i = 0;
    while (current != NULL) {
        REQUIRE(current->interface == expectedInterfaces[i]);
        REQUIRE(current->timestamp.seconds == expectedTimeSeconds[i]);
        REQUIRE(current->timestamp.fracSeconds == expectedTimeFracSeconds[i]);
        REQUIRE(current->can_id == expectedCAN_ID[i]);
        uint64_t num = 0;
        for(int j = 0; j < ceil(current->numBits / 8.0); j++) {
            num = current->data[j] + (num << 8);
        }
        REQUIRE(expectedData[i] == num);
        current = current->next;
        i++;
    }
    
    
    delete head;                         
}


TEST_CASE("Do the sensor values get extracted properly", "[general - unknown]") {
    std::vector<std::string> *lines2 = new std::vector<std::string>;
    pipeInput("../../dbc-files/ControlBus.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc",
            "../test_dump/dump3.log", lines2);
    std::vector<std::string> linesValid = {};
    std::string buff;
    if ((*lines2).size() > 1) {
        for (size_t i{0}; i < lines2->size(); i++) {
        
            lines2->at(i).erase(std::remove(lines2->at(i).begin(), lines2->at(i).end(), '\n'), lines2->at(i).cend());
            //linesValid.push_back((*lines)[i]);
        }
        lines2->erase(lines2->begin());
        
    // second test case has big endian
    // 99% percent that the second given example you give in incorrect 
    // I swear atleast I think the library has fucked something up
        
        std::vector<std::string> expectedOutputs = {"(2639.316999): STEERING_REQ_ROS: -262.23",
                                                    "(2639.316999): STEERING_ACTUAL: 0",
                                                    "(2639.316999): TORQUE_REQ_ROS: 131.07",
                                                    "(2639.316999): VELOCITY_REQ_ROS: -218.46",
                                                    "(2639.317): High_Thermistor_ID: 95",
                                                    "(2639.317): Low_Thermistor_ID: 243",
                                                    "(2639.317): Average_Temperature: 15",
                                                    "(2639.317): Internal_Temperature: 245",
                                                    "(2639.317): Fan_Speed: 240",
                                                    "(2639.317): Fan_Voltage: 616.05",
                                                    "(2639.317): Blank: 170",
                                                    };
    
        REQUIRE(expectedOutputs == (*lines2));      
    }
    delete lines2;                        
}

TEST_CASE("handles the same can_id correctly", "[general - unknown]") {
    // in both Sensor and Control bus there is a can id for 1800 and 1799
    std::vector<std::string> *lines2 = new std::vector<std::string>;
    pipeInput("../../dbc-files/ControlBus.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc",
            "../test_dump/dump4.log", lines2);
    std::vector<std::string> linesValid = {};
    std::string buff;
    if ((*lines2).size() > 1) {
        for (size_t i{0}; i < lines2->size(); i++) {
        
            lines2->at(i).erase(std::remove(lines2->at(i).begin(), lines2->at(i).end(), '\n'), lines2->at(i).cend());
            //linesValid.push_back((*lines)[i]);
        }
        lines2->erase(lines2->begin());
        
    // second test case has big endian
    // 99% percent that the second given example you give in incorrect 
    // I swear atleast I think the library has fucked something up
        
        std::vector<std::string> expectedOutputs = {"(2639.316999): BrakePressure_Rear: 6552",
                                                    "(2639.316999): BrakePressure_Front: 5227.5",
                                                    "(2639.317): Board8_Temp: 3855",
                                                    "(2639.317): Board8_Current2: -16",
                                                    "(2639.317): Board8_Current1: -13261",
                                                    };
    
        REQUIRE(expectedOutputs == (*lines2));      
    }
    delete lines2;                        
}



TEST_CASE("test offset by 1", "[general - unknown]") {
    std::vector<std::string> *lines2 = new std::vector<std::string>;
    pipeInput("../test_dbc/test.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc",
            "../test_dump/dump5.log", lines2);
    std::vector<std::string> linesValid = {};
    std::string buff;
    if ((*lines2).size() > 1) {
        for (size_t i{0}; i < lines2->size(); i++) {
        
            lines2->at(i).erase(std::remove(lines2->at(i).begin(), lines2->at(i).end(), '\n'), lines2->at(i).cend());
            //linesValid.push_back((*lines)[i]);
        }
        lines2->erase(lines2->begin());
        

        
        std::vector<std::string> expectedOutputs = {
                                                    "(2639.5): Board8_Temp: 0",
                                                    "(2639.5): Board8_Current2: 0",
                                                    "(2639.5): Board8_Current1: 16",
                                                    "(2639.6): Board8_Temp: 0",
                                                    "(2639.6): Board8_Current2: 0",
                                                    "(2639.6): Board8_Current1: 8",
                                                    };
    
        REQUIRE(expectedOutputs == (*lines2));      
    }
    delete lines2;                        
}

TEST_CASE("error tests", "[general - unknown]") {
    //here it is assumed if something is wrong the output will just not appear
    // no input i.e #  is all zeros
    // invalid i.e #4787*&& is nothing printed
    // invalid interface i.e NOTVALid is nothing printed
    // invalid can id - nothing printed
    std::vector<std::string> *lines2 = new std::vector<std::string>;
    pipeInput("../test_dbc/test.dbc", "../../dbc-files/SensorBus.dbc", "../../dbc-files/TractiveBus.dbc",
            "../test_dump/dump6.log", lines2);
    std::vector<std::string> linesValid = {};
    std::string buff;
    if ((*lines2).size() > 1) {
        for (size_t i{0}; i < lines2->size(); i++) {
        
            lines2->at(i).erase(std::remove(lines2->at(i).begin(), lines2->at(i).end(), '\n'), lines2->at(i).cend());
        }
        lines2->erase(lines2->begin());
        

        // essentially first log works
        // second doesn't, third should all be zeros, 4 no output (invalid interfece)
        // 5 yes, 6 no, 7 yes
        // the ones that work are just to make sure it still works after
        std::vector<std::string> expectedOutputs = {
                                                    "(2639.5): Board8_Temp: 0",
                                                    "(2639.5): Board8_Current2: 0",
                                                    "(2639.5): Board8_Current1: 16",
                                                    "(2639.5): Board8_Temp: 0",
                                                    "(2639.5): Board8_Current2: 0",
                                                    "(2639.5): Board8_Current1: 0",
                                                    "(2639.5): Board8_Temp: 0",
                                                    "(2639.5): Board8_Current2: 0",
                                                    "(2639.5): Board8_Current1: 16",
                                                    "(2639.5): Board8_Temp: 0",
                                                    "(2639.5): Board8_Current2: 0",
                                                    "(2639.5): Board8_Current1: 16",
                                                    };
    
        REQUIRE(expectedOutputs == (*lines2));      
    }
    delete lines2;                        
}