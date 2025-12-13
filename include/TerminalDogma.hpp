#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#define WIN32_LEAN_AND_MEAN
// CRITICAL: winsock2.h MUST come before windows.h
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <stack>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <sstream>
#include <cstdarg>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iomanip>
#include <csignal>
#include <algorithm>
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iphlpapi.h>

#ifndef ERROR_BUFFER_OVERFLOW
#define ERROR_BUFFER_OVERFLOW 111L
#endif

#ifndef ERROR_INSUFFICIENT_BUFFER
#define ERROR_INSUFFICIENT_BUFFER 122L
#endif

#ifndef NO_ERROR
#define NO_ERROR 0L
#endif

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0L
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
inline const char *inet_ntop_mingw(int af, const void *src, char *dst, size_t size)
{
    if (af == AF_INET)
    {
        struct sockaddr_in in;
        memset(&in, 0, sizeof(in));
        in.sin_family = AF_INET;
        memcpy(&in.sin_addr, src, sizeof(struct in_addr));

        DWORD s = (DWORD)size;
        if (WSAAddressToStringA((LPSOCKADDR)&in, sizeof(in), nullptr, dst, &s) == 0)
        {
            return dst;
        }
    }
    return nullptr;
}
// Override inet_ntop for MinGW
#define inet_ntop(af, src, dst, size) inet_ntop_mingw(af, src, dst, size)
#endif

#endif
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/select.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sysexits.h>
#include <cerrno>
#include <arpa/inet.h>
#endif

#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0

class CentralDogma;

class TerminalDogma
{
public:
    virtual ~TerminalDogma() {};
    virtual void execute(const std::vector<std::string> &args) = 0;
    virtual std::string description() const = 0;
};

class Run : public TerminalDogma
{
    CentralDogma &core;

public:
    Run(CentralDogma &c) : core(c) {};
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Runs the commands stored in <filename>.eva, one by one."; }
};

class Say : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Prints text to the console and also write and appends text into files which are in double quotes only \n\t and ends with >> and file name with proper space. say <text> or say <\"text\"> >> <filename>"; }
};

class Help : public TerminalDogma
{
    CentralDogma &core;

public:
    Help(CentralDogma &c) : core(c) {};
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Gives information about the commands"; };
};

class Calculate : public TerminalDogma
{
    std::unordered_set<char> operators = {'+', '-', '*', '/', '%', '^'};

public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Just a simple calculator."; };
    int precedence(std::string c);
    std::vector<std::string> postfix(std::vector<std::string> args);
    long double solvePostfix(std::vector<std::string> expression);
    bool isNumber(const std::string &s);
    bool isLeftAssociative(char op);
};

class Time : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "It just shows you the time."; };
};

class Clear : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Clears the console."; };
};

class MakeDirectory : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override
    {
#if defined(_WIN32) || defined(_WIN64)
        return "It makes directory. Command - forge <directory1> <directory2> <...>";
#else
        return "It makes directory. Command - forge [-a|-s] <directory1> <directory2> <...>";
#endif
    };
    std::vector<std::string> splitPath(const std::string &path);
};

class ChangeDirectory : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Changes the directory to the given directory."; };
};

class Erase : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Deletes the provided files and folders. erase <file1> <folder1> <file2> <...>"; };
    int removeDir(std::string path);
};

class Show : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Shows all the folder and files present in the working directory."; };
};

class Craft : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Creates files only. craft <file1> <file2> <...>"; };
};

class View : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Displays the content of the files. view <file1> <file2> <...>"; };
};

class Shift : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "Moves and renames files/folders. shift <file1> <folder1/file1> or shift <folder2/folder3> <folder4/folder3>.\n\tYou need to give the name of the file/folder along with the destination you want to move.\n\tFor renaming you just need to give a new name in the second arguement."; };
};

class History : public TerminalDogma
{
    CentralDogma &core;

public:
    History(CentralDogma &c) : core(c) {};
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "It shows all the previous commands executed and also searches commands related with command name(s) given.\n\tTo run a particular command from history with their index - !<index_number>"; };
};

class Sys : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override { return "It can be used to interact with the system and processes. Also it shows specific things related to the system.\n\tsys - shows system and hardware informations.\n\t -p | --proc - shows all the processes.\n\t -n | --network - shows information about your network and it's adapters.\n\t -a | --adapters - shows information about adapters."; };

    // for getting process in windows
#if defined(_WIN64) || defined(_WIN32)
    BOOL GetProcessListWin();
    void PrintError(const TCHAR *msg);
#endif
    void SystemInfo();

// for getting process in linux/unix
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    bool isNumeric(const std::string &s)
    {
        for (char c : s)
            if (!isdigit(c))
                return false;
        return !s.empty();
    }

    void GetProcessesUnix();
#endif

    // for network
#if defined(_WIN64) || defined(_WIN32)
    void GetNetworkInfoWin();

    // shows network adapters and other adapters as well.
    void GetAdapterInfoWin();
#endif

    // for getting network informations and adapters information in linux and other processes
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    void GetNetworkInfoUnix();
    void GetAdapterInfoUnix();
#endif
};

class Probe : public TerminalDogma
{
public:
    void execute(const std::vector<std::string> &args) override;
    std::string description() const override
    {
        return "Network diagnostics tool with real-time latency visualization.\n"
               "\tprobe <host> [-c count] [-t] [-i interval]\n"
               "\t  -c <count>   : Number of packets to send (default: 4)\n"
               "\t  -t           : Continuous ping\n"
               "\t  -i <ms>      : Interval between pings in ms (default: 1000)";
    }

private:
    struct PingStats
    {
        int sent = 0;
        int received = 0;
        int lost = 0;
        double minTime = 9999.0;
        double maxTime = 0.0;
        double totalTime = 0.0;
        std::vector<double> latencies;
    };

    void displayHeader(const std::string &host, const std::string &ip);
    void displayResult(int seq, double latency, const std::string &ip, int ttl);
    void displayStats(const PingStats &stats, const std::string &host);
    void displayGraph(const std::vector<double> &latencies);
    std::string resolveHostname(const std::string &host);
    std::string getSignalQuality(double avgLatency);

    // Use raw sockets on all platforms (MinGW compatible)
    bool pingRawSocket(const std::string &ip, double &latency, int &ttl);
    unsigned short checksum(void *b, int len);

    // ICMP packet structure
    struct ICMPHeader
    {
        unsigned char type;
        unsigned char code;
        unsigned short checksum;
        unsigned short id;
        unsigned short seq;
    };
};