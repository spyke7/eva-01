#include "TerminalDogma.hpp"
#include "CentralDogma.hpp"
#include <iostream>

void Say ::execute(const std::vector<std::string> &args)
{
    std::string filePath;
    if (args[args.size() - 2] == ">>" && args.size() >= 4)
    {
        if (args[1][0] == '"' && args[args.size() - 3][args[args.size() - 3].size() - 1] == '"' )
        {
            filePath = args[args.size() - 1];
        }
    }

    if (filePath.size() == 0)
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            std::cout << args[i] << " ";
        }
        std::cout << "\n";
    }
    else
    {
        std::string userText;
        for (size_t i = 1; i <= args.size()-3; i++)
        {
            for (size_t j=0; j<args[i].size(); j++){
                if ((i==1 && j==0) || (i==args.size()-3 && j==args[args.size()-3].size()-1)){
                    continue;
                }
                else{
                    userText+=args[i][j];
                }
            }
        }
        std::cout<<userText<<"\n";
        std::ofstream file(filePath, std::ios::app);
        if (!file){
            std::fstream file(filePath, std::ios::out);
            file << userText;
        }
        else{
            file<<userText;
        }
        file.close();
    }
}

void Help::execute(const std::vector<std::string> &args)
{
    std::cout << "Available commands:\n";
    for (const auto &name : core.getRegistry())
    {
        std::cout << "  " << name.first << " - " << name.second->description() << "\n";
    }
}

bool Calculate::isNumber(const std::string &s)
{
    if (s.empty())
        return false;

    bool hasDecimal = false;
    for (char c : s)
    {
        if (c == '.')
        {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        }
        else if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool Calculate::isLeftAssociative(char op)
{
    if (op == '^')
        return false;
    return true;
}

int Calculate::precedence(std::string c)
{
    if (c == "^")
        return 3;
    else if (c == "/" || c == "*")
        return 2;
    else if (c == "+" || c == "-")
        return 1;
    else
        return -1;
}

std::vector<std::string> Calculate::postfix(std::vector<std::string> args)
{
    std::stack<std::string> st;
    std::vector<std::string> result;

    for (int i = 0; i < args.size(); i++)
    {
        if (isNumber(args[i]))
        {
            result.push_back(args[i]);
        }
        // handling -> ()
        else if (args[i] == "(")
        {
            st.push(args[i]);
        }
        else if (args[i] == ")")
        {
            while (!st.empty() && st.top() != "(")
            {
                result.push_back(st.top());
                st.pop();
            }
            if (!st.empty())
                st.pop();
        }

        // in case of operators
        else if (operators.count(args[i][0]))
        {
            while (!st.empty() && operators.count(args[i][0]))
            {
                if ((isLeftAssociative(args[i][0]) && precedence(args[i]) <= precedence(st.top())) ||
                    (!isLeftAssociative(args[i][0]) && precedence(args[i]) < precedence(st.top())))
                {
                    result.push_back(st.top());
                    st.pop();
                }
                else
                {
                    break;
                }
            }
            st.push(args[i]);
        }
    }

    while (!st.empty())
    {
        result.push_back(st.top());
        st.pop();
    }

    return result;
}

long double Calculate::solvePostfix(std::vector<std::string> expression)
{
    std::stack<long double> st;
    for (int i = 0; i < expression.size(); i++)
    {
        if (isNumber(expression[i]))
        {
            long double num = std::stold(expression[i]);
            st.push(num);
        }
        else
        {
            long double val1 = st.top();
            st.pop();
            long double val2 = st.top();
            st.pop();
            long double res;
            if (expression[i] == "+")
            {
                res = val1 + val2;
            }
            else if (expression[i] == "-")
            {
                res = val2 - val1;
            }
            else if (expression[i] == "*")
            {
                res = val1 * val2;
            }
            else if (expression[i] == "/")
            {
                res = val2 / val1;
            }
            else if (expression[i] == "^")
            {
                res = pow(val2, val1);
            }
            st.push(res);
        }
    }
    return st.top();
}

void Calculate::execute(const std::vector<std::string> &args)
{
    std::string expression;
    for (size_t i = 1; i < args.size(); i++)
    {
        expression += args[i];
    }

    if (expression.size() > 2)
    {

        bool safety = true;
        long long nbraces = 0;

        // tokens vector after filtering user input
        std::vector<std::string> tokens;
        std::string token = "";

        int i = 0;
        while (i < expression.size() && safety)
        {
            if ((expression[i] >= '0' && expression[i] <= '9') || expression[i] == '.')
            {
                while (!operators.count(expression[i]) && (expression[i] != '(') && (expression[i] != ')') && i < expression.size())
                {
                    token += expression[i];
                    i++;
                }
                safety = isNumber(token);
                if (safety)
                {
                    tokens.push_back(token);
                }
                token = "";
            }
            else if (expression[i] == '(' || expression[i] == ')' || operators.count(expression[i]))
            {
                token += expression[i];
                tokens.push_back(token);
                if (expression[i] == '(' || expression[i] == ')')
                {
                    nbraces++;
                }

                token = "";
                i++;
            }
            else
            {
                safety = false;
                break;
            }
        }

        // validating signs and brackets
        if (nbraces % 2 != 0)
        {
            safety = false;
        }
        else
        {
            for (int i = 0; i < tokens.size() - 1; i++)
            {
                if (tokens[i] == "/" && tokens[i + 1] == "0")
                {
                    safety = false;
                    break;
                }
                // handling negative sign before making the safety false for other cases
                else if (tokens[i] == "-")
                {
                    if (i == 0 || (tokens[i - 1] == "(" && i > 0))
                    {
                        tokens.insert(tokens.begin() + i, "0");
                    }
                    else if (operators.count(tokens[i - 1][0]) && tokens[i - 1] != "-")
                    {
                        tokens.insert(tokens.begin() + i, "(");
                        tokens.insert(tokens.begin() + i + 1, "0");
                        int k = i + 3;

                        while (k < tokens.size() &&
                               !operators.count(tokens[k][0]) && tokens[k] != ")" && tokens[k] != "(" && tokens[k] != ".")
                        {
                            k++;
                        }

                        tokens.insert(tokens.begin() + k, ")");
                    }
                    else if (operators.count(tokens[i + 1][0]))
                    {
                        safety = false;
                        break;
                    }
                    continue;
                }
                else if (operators.count(tokens[0][0]) && tokens[i] != "-")
                {
                    safety = false;
                }
                else if (operators.count(tokens[tokens.size() - 1][0]))
                {
                    safety = false;
                }
                else if (operators.count(tokens[i][0]) && operators.count(tokens[i + 1][0]) && tokens[i + 1] != "-")
                {
                    safety = false;
                }
                else if ((operators.count(tokens[i][0]) && (tokens[i + 1] == ")")) || (tokens[i] == "(") && operators.count(tokens[i + 1][0]) && (tokens[i + 1] != "-"))
                {
                    safety = false;
                }
            }
        }

        if (!safety)
        {
            std::cout << "Give Correct expression\n";
        }
        else
        {
            std::cout << solvePostfix(postfix(tokens)) << "\n";
        }
    }
}

void Time ::execute(const std::vector<std::string> &args)
{
    if (args.size() > 1)
    {
        std::cout << "time takes no arguement!\n";
        return;
    }
    std::time_t now = std::time(nullptr);
    std::string datetime = std::ctime(&now);
    datetime.pop_back();
    std::cout << datetime << "\n";
}

void Clear ::execute(const std::vector<std::string> &args)
{
    if (args.size() > 1)
    {
        std::cout << "clear takes no arguement!\n";
        return;
    }
#if defined(_WIN32) || defined(_WIN64)
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
#else
    std::cout << "\033[H\033[2J\033[3J" << std::flush;
#endif
}

std::vector<std::string> MakeDirectory ::splitPath(const std::string &path)
{
    std::vector<std::string> paths;
    std::string token;
    for (size_t i = 0; i < path.size(); i++)
    {
        if (path[i] != '/')
            token += path[i];
        else
        {
            if (!token.empty())
            {
                paths.push_back(token);
                token.clear();
            }
        }
    }

    if (!token.empty())
        paths.push_back(token);

    return paths;
}

void MakeDirectory ::execute(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cout << "forge takes 2 or more arguements. See help.\n";
        return;
    }

#if defined(_WIN32) || defined(_WIN64)
    for (size_t i = 1; i < args.size(); i++)
    {
        std::string tmp = args[i];
        std::vector<std::string> paths = splitPath(tmp);
        std::string currentPath;

        if (!tmp.empty() && tmp[0] == '/')
            currentPath = "/";

        for (size_t i = 0; i < paths.size(); ++i)
        {
            currentPath += paths[i];
            if (i != paths.size() - 1)
                currentPath += "\\";

            if (mkdir(currentPath.c_str()) == -1)
            {
                std::cout << "Cannot create - " << tmp << "\n";
                break;
            }
        }
    }

#else
    size_t index = 1;
    mode_t mode = 0755; // default permission
    while (index < args.size() && args[index][0] == '-')
    {
        if (args[index] == "-a")
        {
            mode = 0777; // access to all
        }
        else if (args[index] == "-s")
        {
            mode = 0700; // acess to owner only
        }
        else
        {
            std::cout << "forge: unknown option " << args[index] << "'\n";
        }
        index++;
    }
    if (index >= args.size())
    {
        std::cout << "forge: missing directory operand\n";
        return;
    }
    for (size_t i = index; i < args.size(); i++)
    {
        std::string dirPath = args[i];
        std::vector<std::string> paths = splitPath(dirPath);
        std::string currentPath;

        if (!dirPath.empty() && dirPath[0] == '/')
            currentPath = "/";
        for (size_t i = 0; i < paths.size(); ++i)
        {
            currentPath += paths[i];
            if (i != paths.size() - 1)
                currentPath += "/";

            if (mkdir(currentPath.c_str()) == -1)
            {
                std::cout << "Cannot create - " << tmp << "\n";
                break;
            }
        }
    }

#endif
}

void ChangeDirectory ::execute(const std::vector<std::string> &args)
{
    std::string tmp;
    for (int i = 1; i < args.size(); i++)
    {
        tmp += args[i];
    }

    if (chdir(tmp.c_str()) != 0)
    {
        std::cout << "Cannot change directory to " << tmp << "\n";
    }
}

int Erase::removeDir(std::string path)
{
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return -1;

    struct dirent *entity;
    while ((entity = readdir(dir)) != nullptr)
    {
        if (strcmp(entity->d_name, ".") == 0 || strcmp(entity->d_name, "..") == 0)
        {
            continue;
        }

        std::string fullPath = path + "/" + entity->d_name;

        // checking for files and folders
        struct stat buf;
        if (stat(fullPath.c_str(), &buf) == 0)
        {
            if (S_ISDIR(buf.st_mode))
            {
                if (removeDir(fullPath) != 0)
                {
                    std::cout << "Cannot delete - " << fullPath << "\n";
                    closedir(dir);
                    return -1;
                }
            }
            else
            {
                if (remove(fullPath.c_str()) != 0)
                {
                    std::cout << "Cannot delete - " << fullPath << "\n";
                    closedir(dir);
                    return -1;
                }
            }
        }
    }

    closedir(dir);
    return rmdir(path.c_str());
}

void Erase::execute(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cout << "erase takes 2 or more arguements. See help.\n";
    }

    for (size_t i = 1; i < args.size(); i++)
    {
        // checking for files and folders
        struct stat buf;
        if (stat(args[i].c_str(), &buf) == 0)
        {
            if (S_ISDIR(buf.st_mode))
            {
                // checking and recursively deleteing file and folder using the removeDir()

                if (removeDir(args[i]) != 0)
                {
                    std::cout << "Cannot delete - " << args[i] << "\n";
                }
            }
            else
            {
                if (remove(args[i].c_str()) != 0)
                {
                    std::cout << "Cannot delete - " << args[i] << "\n";
                }
            }
        }
        else
        {
            std::cout << "See help.\n";
        }
    }
}

void Show ::execute(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
        {
            std::cout << "Working Directory - " << cwd << "\n";
        }

        DIR *dir;
        dir = opendir(".");
        if (dir != nullptr)
        {
            struct dirent *entity;
            entity = readdir(dir);
            while (entity != nullptr)
            {
                std::cout << entity->d_name << " ";
                entity = readdir(dir);
            }
            std::cout << "\n";
            closedir(dir);
        }
    }
    else
    {
        std::cout << "show takes no arguement.\n";
    }
}

void Craft ::execute(const std::vector<std::string> &args)
{
    for (size_t i = 1; i < args.size(); i++)
    {
        // checking if the file exists
        std::fstream file(args[i], std::ios::in);
        if (!file.is_open())
        {
            std::ofstream file(args[i]);
        }
        file.close();
    }
}

void View ::execute(const std::vector<std::string> &args)
{
    for (size_t i = 1; i < args.size(); i++)
    {
        std::fstream file(args[i], std::ios::in);
        if (file.is_open())
        {
            std::cout << args[i] << " - \n";
            std::ifstream file(args[i]);
            std::string line;
            while (std::getline(file, line))
                std::cout << line << "\n";
        }
        else
        {
            std::cout << args[i] << " - doesn't exists!\n";
        }
        file.close();
    }
}

void Shift :: execute(const std::vector<std::string> &args){
    if (args.size()!=3){
        std::cout<<"shift takes 3 arguements\n";
    }
    std::string src = args[1];
    std::string dest = args[2];

    if (rename(src.c_str(), dest.c_str())!=0){
        std::cout<<"Unable to do the operation\n";
    }
}

void History :: execute(const std::vector<std::string> &args){
    if (args.size()>=2){
        for (int i=1; i<args.size(); i++){
            core.searchHistory(args[i]);
        }
    }else{
        std::vector<std::vector<std::string>> commandList = core.getHistory();
        for (int i = 0; i < commandList.size(); i++)
        {
            std::cout<<i+1<<".  ";
            for (int j = 0; j < commandList[i].size(); j++)
            {
                std::cout<<commandList[i][j]<<" ";
            }
            std::cout<<"\n";
        }
        
    }
}