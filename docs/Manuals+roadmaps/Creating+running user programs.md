# Creating and Running a User Program in AthenX

Follow these steps to build, run, and load a user program in AthenX:

1. **Build Cross-Compiler**

   Open your terminal and execute the following command to build the cross-compiler for AthenX:

   ```bash
   ./build-toolchain.sh
2. **Add Toolchain Bin to PATH**

    After building the toolchain, you need to add the toolchain's bin folder to your system's PATH. This allows your system to find and use the cross-compiler.

    Instructions for adding the bin folder to your PATH may vary depending on your system. Typically, you can add it by editing your shell profile configuration file (e.g., `~/.bashrc` or `~/.zshrc`) and appending the following line:
    ```bash
    export PATH=/path/to/your/toolchain/bin:$PATH
    ```
    Replace /path/to/your/toolchain with the actual path to your toolchain's bin folder.
3. **Open User dictionary***
    
    This is the dictionary named user in the file you cloned from github
4. **Create a New Folder for Your Program**

    Inside your user directory, create a new folder with the name of your program. This folder will be the workspace for your program's source code.
5. **Create Code for Your Program**

    Write your program's source code within the folder you created in the previous step. See this guide for more information about the [`libc`](libc.md#introduction) for AthenX
6. **Go to Main Directory and Build**

    In the terminal, navigate to the main directory of AthenX and use the make run command to boot AthenX into the shell:
    ```bash
    make run
    ```
7. **Load Your Program**
    
    Once AthenX has booted into the shell, you can load and run your program using the following command:
    ```bash
    load "program name"
    ```
    Replace "program name" with the actual name of your program.
That's it! You've successfully built, run, and loaded a user program in AthenX.