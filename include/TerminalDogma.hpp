#pragma once
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
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <winsock2.h>
#include <iphlpapi.h>
#endif
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sysexits.h>
#include <cerrno>
#include <arpa/inet.h>
#endif

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