#include <iostream>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <sstream>
#include "Diagnostics.h"
#define NOMINMAX
#include <Windows.h>

using namespace std;


void setColor(int code) // for color
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), code);
}

string getTimestamp() // for current time
{
    time_t now = time(nullptr);
    tm timeInfo;
    localtime_s(&timeInfo, &now); // safe
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %I:%M:%S %p", &timeInfo);
    return string(buf);
}

void exportReport() // save report to .txt file
{
    ofstream file("system_report.txt");
    if (!file) 
    {
        cout << "Error writing report.\n";
        return;
    }

    file << "====== System Stats Tool ======\n";
    file << "Generated: " << getTimestamp() << "\n\n";
    file << "[System Info]\n" << runCommand("ver") << "\n";
    file << "[OS Version]\n" << runCommand("systeminfo | findstr /C:\"OS Version\"") << "\n";
    file << "[CPU Info]\n" << runCommand("powershell -Command \"Get-CimInstance Win32_Processor | Select-Object -ExpandProperty Name\"") << "\n";
    file << "[GPU Info]\n" << runCommand("powershell -Command \"Get-CimInstance Win32_VideoController | Select-Object -ExpandProperty Name\"") << "\n";
    file << "[RAM Info]\n" << runCommand("systeminfo | findstr /C:\"Total Physical Memory\"") << "\n";
    file << "[Disk Info]\n" << runCommand("powershell -Command \"Get-Volume | Format-Table DriveLetter, SizeRemaining, Size\"") << "\n";
    file << "[IP Address]\n" << runCommand("ipconfig | findstr /C:\"IPv4\"") << "\n";
    file << "[BIOS Serial]\n" << runCommand("powershell -Command \"(Get-CimInstance Win32_BIOS).SerialNumber\"") << "\n";

    file.close();
    setColor(10); // green
    cout << "\n[OK] Report saved to system_report.txt\n";
    setColor(15);
}


void emailReport() 
{
    string mailCommand = R"(start msedge "https://mail.google.com/mail/?view=cm&fs=1&to=&su=System%20Report&body=See%20attached%20system_report.txt")";
    system(mailCommand.c_str());
    setColor(14); // yellow
    cout << "\n[INFO] Opened email client. Attach the file manually.\n";
    setColor(15);
}

void printCentered(const string& text, int width = 80) 
{
    int pad = (width - static_cast<int>(text.length())) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << text << endl;
}

void printBlockCentered(const std::string& block, int width = 80)
{
    std::istringstream stream(block);
    std::string line;
    while (std::getline(stream, line))
    {
        int pad = (width - static_cast<int>(line.length())) / 2;
        if (pad < 0) pad = 0;
        cout << string(pad, ' ') << line << endl;
    }
}

void displayDashboard() // display the dashboard UI
{
    system("cls");

    string timeStr = getTimestamp();
    string os = runCommand("ver");
    string osVer = runCommand("systeminfo | findstr /C:\"OS Version\"");
    string cpu = runCommand("powershell -Command \"Get-CimInstance Win32_Processor | Select-Object -ExpandProperty Name\"");
    string gpu = runCommand("powershell -Command \"Get-CimInstance Win32_VideoController | Select-Object -ExpandProperty Name\"");
    string ram = runCommand("systeminfo | findstr /C:\"Total Physical Memory\"");
    string disk = runCommand("powershell -Command \"Get-Volume | Format-Table DriveLetter, SizeRemaining, Size\"");
    string ip = runCommand("ipconfig | findstr /C:\"IPv4\"");
    string serial = runCommand("powershell -Command \"(Get-CimInstance Win32_BIOS).SerialNumber\"");

    setColor(11); // light cyan
    printCentered("=======================================");
    printCentered("       System Stats Tool         ");
    printCentered(timeStr);
    printCentered("=======================================\n");
    setColor(15); // white

    // system info
    setColor(10); printCentered("[SYSTEM INFO]"); setColor(15);
    printBlockCentered(os);
    printBlockCentered(osVer);

    setColor(10); printCentered("[CPU]"); setColor(15);
    printBlockCentered(cpu);

    setColor(10); printCentered("[GPU]"); setColor(15);
    printBlockCentered(gpu);

    setColor(10); printCentered("[RAM]"); setColor(15);
    printBlockCentered(ram);

    setColor(10); printCentered("[DISK]"); setColor(15);
    printBlockCentered(disk);

    setColor(10); printCentered("[IP ADDRESS]"); setColor(15);
    printBlockCentered(ip);

    setColor(10); printCentered("[BIOS SERIAL]"); setColor(15);
    printBlockCentered(serial);

    cout << endl;
    setColor(14); // yellow
    printCentered("[E] Export Report    [M] Email Report    [Q] Quit");
    setColor(15);
}

int main()
{
    char input;
    bool skipRefresh = false;

    do
    {
        if (!skipRefresh)
            displayDashboard();

        input = tolower(_getch()); // wait for key press
        skipRefresh = false; // reset by default

        if (input == 'e')
        {
            exportReport();
            system("pause");
            skipRefresh = true;
        }
        else if (input == 'm')
        {
            emailReport();
            system("pause");
            skipRefresh = true;
        }

    } while (input != 'q');

    setColor(9); // blue
    cout << '\n';
    printCentered("Exiting...\n");
    setColor(15);
    return 0;
}