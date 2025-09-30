#include "CentralDogma.hpp"
#include "TerminalDogma.hpp"
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& command){
    std::vector<std::string> tokens;
    std::string token;
    for (size_t i=0; i<command.length(); i++){
        if (command[i]!=' '){
            token+=command[i];
        }
        else{
            if (token.size() != 0){
                tokens.push_back(token);
                token.clear();
            }
        }
    }
    if (token.size()!=0) tokens.push_back(token);

    return tokens;
}


int main(){
    CentralDogma core;

    // registering the commands
    core.registerCommand("say", std::make_unique<Say>());
    core.registerCommand("help", std::make_unique<Help>(core));
    core.registerCommand("calc", std::make_unique<Calculate>());
    core.registerCommand("time", std::make_unique<Time>());
    core.registerCommand("clear", std::make_unique<Clear>());
    core.registerCommand("jump", std::make_unique<ChangeDirectory>());
    core.registerCommand("forge", std::make_unique<MakeDirectory>());
    core.registerCommand("show", std::make_unique<Show>());
    core.registerCommand("erase", std::make_unique<Erase>());
    core.registerCommand("craft", std::make_unique<Craft>());
    core.registerCommand("view", std::make_unique<View>());
    core.registerCommand("shift", std::make_unique<Shift>());
    core.registerCommand("history", std::make_unique<History>(core));


    // getting credentials
    std::string username, password;
    username = core.getUsername();

    std::string command;
    while (true) {
        std::string cwd = core.workingDirectory();
        std::cout << "[EVA-01]::"<<username<<" " << cwd << ">";

        std::getline(std::cin, command);
        std::vector<std::string> args = split(command);

        if (args[0] == "exit") break;

        if (!core.parseCommand(args)) {
            std::cout << "Unknown command: " << args[0] << "\n";
        }
    }
    return 0;
}