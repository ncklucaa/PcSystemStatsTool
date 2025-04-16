#include "Diagnostics.h"
#include <array>
#include <cstdio>

std::string runCommand(const std::string& command) // uses _popen to run the command, captures output
{
    std::array<char, 256> buffer;
    std::string result;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) return "Error executing command.";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) // read command output to buffer
    {
        result += buffer.data();
    }

    _pclose(pipe);
    return result;
}