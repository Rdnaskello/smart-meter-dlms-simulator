#include "CLI.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

Config ParseArguments(int argc, char* argv[]) {
    Config config;

    int opt;
    while ((opt = getopt(argc, argv, "t:p:n:")) != -1) {
        switch (opt) {
        case 't':
            if (strcmp(optarg, "Error") == 0)
                config.trace = TraceLevel::Error;
            else if (strcmp(optarg, "Warning") == 0)
                config.trace = TraceLevel::Warning;
            else if (strcmp(optarg, "Info") == 0)
                config.trace = TraceLevel::Info;
            else if (strcmp(optarg, "Verbose") == 0)
                config.trace = TraceLevel::Verbose;
            else if (strcmp(optarg, "Off") == 0)
                config.trace = TraceLevel::Off;
            else {
                std::cerr << "Invalid trace option: " << optarg << std::endl;
                exit(1);
            }
            break;
        case 'p':
            config.startPort = std::stoi(optarg);
            break;
        case 'n':
            config.count = std::stoi(optarg);
            if (config.count <= 0) {
                std::cerr << "Invalid meter count: " << optarg << std::endl;
                exit(1);
            }
            break;
        case '?':
        default:
            std::cout << "Usage: ./smart_meter_simulator -p [port] -n [count] -t [trace]\n";
            exit(0);
        }
    }

    return config;
}
