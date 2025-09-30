#include "CentralDogma.hpp"
#include "TerminalDogma.hpp"

CentralDogma::CentralDogma()
{

#if defined(_WIN32) || defined(_WIN64)
    char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(hostname);
    if (GetComputerNameA(hostname, &size))
    {
        username = std::string(hostname);
    }
#else
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        username = std::string(hostname);
    }
#endif
}

void CentralDogma::registerCommand(const std::string &name, std::unique_ptr<TerminalDogma> cmd)
{
    registry[name] = std::move(cmd);
}

bool CentralDogma::executeCommand(const std::string &name, const std::vector<std::string> &args)
{
    auto it = registry.find(name);
    if (it != registry.end())
    {
        it->second->execute(args);
        return true;
    }
    return false;
}

std::string CentralDogma::getUsername()
{
    return username;
}

std::string CentralDogma::workingDirectory()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr)
    {
        std::string path = cwd;
        size_t pos = path.find_last_of("/\\");
        std::string lastDir;
        if (pos != std::string::npos)
        {
            lastDir = path.substr(pos + 1);
        }
        else
        {
            lastDir = path;
        }
        return lastDir;
    }
    return "";
}

void CentralDogma::addToHistory(const std::vector<std::string> &args)
{

    historyList.push_back(args);
}

bool CentralDogma::parseCommand(const std::vector<std::string> &args)
{
    if (args.empty())
        return true;

    std::vector<std::vector<std::string>> commands;
    std::vector<std::string> commandOperators;
    std::vector<std::string> currentCommand;
    std::string currentToken;

    bool insideQuotes = false;

    // ; - executes all commands one after other, || - run the second command only if first fails, && - run the second if the first succeeds

    for (const std::string &token : args)
    {
        size_t i = 0;
        while (i < token.size()) {
            if (token[i] == '"' && !insideQuotes) {
                insideQuotes = true;
                i++;
                continue;
            } else if (token[i] == '"' && insideQuotes) {
                insideQuotes = false;
                i++;
                continue;
            }

            // checking for the operators when quotes are closed
            if (!insideQuotes){
                if (i+1<token.size() && ((token[i] == '&' && token[i + 1] == '&') || (token[i] == '|' && token[i + 1] == '|'))){
                    if (!currentToken.empty()) {
                        currentCommand.push_back(currentToken);
                        currentToken.clear();
                    }
                    if (!currentCommand.empty()) {
                        commands.push_back(currentCommand);
                        currentCommand.clear();
                    }
                    commandOperators.push_back(token.substr(i, 2));
                    i += 2;
                    continue;
                }
                if (token[i] == ';') {
                    if (!currentToken.empty()) {
                        currentCommand.push_back(currentToken);
                        currentToken.clear();
                    }

                    if (!currentCommand.empty()) {
                        commands.push_back(currentCommand);
                        currentCommand.clear();
                    }

                    commandOperators.push_back(";");
                    i++;
                    continue;
                }
            }
            currentToken.push_back(token[i]);
            i++;
        }

        // if inside quotes, adding a space to join with next token
        if (insideQuotes) {
            currentToken.push_back(' ');
        } else {
            if (!currentToken.empty()) {
                currentCommand.push_back(currentToken);
                currentToken.clear();
            }
        }
    }

    if (!currentToken.empty()) {
        currentCommand.push_back(currentToken);
        currentToken.clear();
    }
    if (!currentCommand.empty()) {
        commands.push_back(currentCommand);
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        for (size_t j = 0; j < commands[i].size(); j++)
        {
            std::cout<<commands[i][j]<<" ";
        }
        std::cout<<"\n";
        
    }

    for (size_t i = 0; i < commandOperators.size(); i++)
    {
        std::cout<<commandOperators[i]<<std::endl;
        std::cout<<"\n";
        
    }
    

    // if (!executeCommand(args[0], args))
    // {
    //     addToHistory(args);
    //     return false;
    // }
    // else
    // {
    //     addToHistory(args);
    //     return true;
    // }
}

std::vector<std::vector<std::string>> CentralDogma::getHistory()
{
    return historyList;
}

void CentralDogma::searchHistory(const std::string &cmdName)
{
    for (int i = 0; i < historyList.size(); i++)
    {
        if (historyList[i][0] == cmdName)
        {
            std::cout << i + 1 << ". ";
            for (int j = 0; j < historyList[i].size(); j++)
            {
                std::cout << historyList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
}