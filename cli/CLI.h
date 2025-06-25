#pragma once

enum class TraceLevel {
    Error,
    Warning,
    Info,
    Verbose,
    Off
};

struct Config {
    int count = 1;
    int startPort = 4060;
    TraceLevel trace = TraceLevel::Info;
};

Config ParseArguments(int argc, char* argv[]);
