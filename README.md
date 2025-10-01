<a name="readme-top"></a>
![](https://img.shields.io/maintenance/yes/2025)
![](https://img.shields.io/readthedocs/cplusplus)
![](https://img.shields.io/github/stars/spyke7/eva-01)
![](https://img.shields.io/github/last-commit/spyke7/eva-01)
[![CodeFactor](https://www.codefactor.io/repository/github/spyke7/eva-01/badge)](https://www.codefactor.io/repository/github/spyke7/eva-01)
![](https://img.shields.io/github/license/spyke7/eva-01)
![](https://img.shields.io/github/issues/spyke7/eva-01)

<p align="left"> <a href="https://www.gnu.org/software/bash/" target="_blank"> <img src="https://www.vectorlogo.zone/logos/gnu_bash/gnu_bash-icon.svg" alt="bash" width="40" height="40"/> </a>

<div align="center">
<h1 align="center">🟣 EVA-01 Shell</h1>
The <b>Eva-01 Shell</b> is an immersive CLI experience.  
Inspired by **NERV and EVAs** from _Neon Genesis Evangelion_.

Eva-01 features:

<li> Clean, organized, and minimal. </li>
<li> Faster command parsing & execution. </li>
<li> Built to be <b>developer-friendly</b>, <b>extendable</b>, and <b>modular</b>. </li>
<br/>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#features">FEATURES</a>
    </li>
    <li>
      <a href="#documentation">DOCUMENTATION</a>
      <ul>
        <li><a href="#commands">COMMANDS</a></li>
		<li><a href="#internet">INTERNET</a></li>
      </ul>
    </li>
    <li><a href="#getting-sarted">GETTING STARTED</a>
		<ul>
			<li><a href=#screenshot>SCREENSHOT</a>
		</ul>
	</li>
    <li><a href="#contributing">CONTRIBUTING</a></li>
    <li><a href="#support">SUPPORT</a></li>
    <li><a href="#license">LICENSE</a></li>
  </ol>
</details>

## 🚀 **Features**

- Light Weight
- Fast
- Cross-Platform
- User friendly

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## **Documentation**

I have seen the **Neon Genesis Evangelion** anime by Hideaki Anno, and was really moved by the concepts of EVAs. EVA-01 run by the protagonist was a special one for me as it could change to berserk mode and many other different capabilites. Also, I was moved by the NERV HQ as mentioned in anime and its computer system having central dogma and terminal dogma.
</br>

Based on this computer system mentioned in anime, the eva-01 also has two header files - `CentralDogma.hpp` and `TerminalDogma.hpp`. 
### **About the CentralDogma**
- The main `parseCommand()` function, is used to execute commands, store them in history, also the logic for `||`,`;`, and `&&` are implemented. Along with that the command to run a command from history with its index is also implemented - `!<index_number>`
- Inside it has a registry keeping the records of the commands along with the names with which they will be called. The registry filling is done inside the main function.
- `searchHistory()` function is used by `history` function inside the `TerminalDogma` to search particular commands with related names.

### **About the TerminalDogma**

- It has a brief abstract base class for all eva-01 shell commands. It defines the common interface for every shell command. Each command has its own behaviour in `execute()` and the `description()` is written inside the header file only.
- All the commands implemented inside it are described in the commands section.
- For the `calc()` function many other functions like `isNumber()`, `isLeftAssociative()`, `precedence()`, `postfix()` - to change infix to postfix along with brackets, `solvePostfix()` are implemented.
  

### Commands:
- [x] history - It shows all the previous commands executed and also searches commands related with command name(s) given. To run a particular command from history with their index - `!<index_number>`
- [x] shift - Moves and renames files/folders. `shift <file1> <folder1/file1>` or `shift <folder2/folder3> <folder4/folder3>`
        You need to give the name of the file/folder along with the destination you want to move.
        For renaming you just need to give a new name in the second arguement.
- [x] view - Displays the content of the files. `view <file1> <file2> <...>`
- [x] time - It just shows you the time.
- [x] forge - It makes directory. Command - `forge <directory1> <directory2> <...>`
- [x] calc - Just a simple calculator.
- [x] say - Prints text to the console and also write and appends text into files which are in double quotes only
         and ends with >> and file name with proper space. `say <text>` or `say <"text"> >> <filename>`
- [x] clear - Clears the console.
- [x] show - Shows all the folder and files present in the working directory.
- [x] jump - Changes the directory to the given directory.
- [x] help - Gives information about the commands
- [x] erase - Deletes the provided files and folders. `erase <file1> <folder1> <file2> <...>`
- [x] craft - Creates files only. `craft <file1> <file2> <...>`
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Internet


<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Important:

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## <br/>

## **GETTING STARTED**


## **CONTIBUTING**

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b CommandConsole`)
3. Commit your Changes (`git commit -m 'Add some advices'`)
4. Push to the Branch (`git push origin CommandConsole`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## **SUPPORT**
Support me by giving a ✨ to this project

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## LICENSE
The license is GNU public license see more n LICENSE

<p align="right">(<a href="#readme-top">back to top</a>)</p>

**This is all the things about my program. If you like it, then please share with others. This is a project which the begineers as well as intermediate programmers can build. Support me by following my github account. Thank you.**
