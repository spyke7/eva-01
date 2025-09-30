#pragma once
#include <iostream>
#include <memory>
#include <vector>
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
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#endif

class CentralDogma;

class TerminalDogma
{
public:
    virtual ~TerminalDogma() {};
    virtual void execute(const std::vector<std::string> &args) = 0;
    virtual std::string description() const = 0;
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
    std::string description() const override { return "Searches commands from history."; };
};