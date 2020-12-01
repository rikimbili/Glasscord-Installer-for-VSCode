/* 
This is a glasscord installer/reinstaller for Visual Studio Code and Discord(not supported at the moment). 
The purpose of this program is to copy/create necessary files whenever you execute it in the case that a 
software update breaks your glasscord integration. It is also designed for new users that just want to get 
glasscord installed without the hassle of manually doing each thing.

+-+-+-+-+-+-+-+-+-+-+-> TODO: <-+-+-+-+-+-+-+-+-+-+-+
-Separate redundant operations into functions
-Create .css file in the vs code user dir if not present
-Discord support and ability to choose desired program
-Add Discord installation checks
-Linux support... maybe?
-Implement a GUI?
*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#define BUFFER_SIZE 8192
#define PATH_SIZE 256

void terminate(char *msgStr, int exitCode);
void spinner(int time_sec);

int main()
{
    FILE *ifp, *ofp, *gcordfp, *o_gcordfp, *file_check;
    char PATH[PATH_SIZE], gcordPATH[PATH_SIZE];
    char currentBUFF[BUFFER_SIZE];
    const char searchSTR[] = {"\"main\":"}, asarSTR[] = {"  \"main\": \"./glasscord.asar\",\n"}, *dir_check;

    // ASCII title
    printf("\t  ______ _                                     _    _____                      _ _             \n");
    printf("\t / _____) |                                   | |  (_____)           _        | | |            \n");
    printf("\t| /  ___| | ____  ___  ___  ____ ___   ____ _ | |     _   ____   ___| |_  ____| | | ____  ____ \n");
    printf("\t| | (___) |/ _  |/___)/___)/ ___) _ \\ / ___) || |    | | |  _ \\ /___)  _)/ _  | | |/ _  )/ ___)\n");
    printf("\t| \\____/| ( ( | |___ |___ ( (__| |_| | |  ( (_| |   _| |_| | | |___ | |_( ( | | | ( (/ /| |    \n");
    printf("\t \\_____/|_|\\_||_(___/(___/ \\____)___/|_|   \\____|  (_____)_| |_(___/ \\___)_||_|_|_|\\____)_|    \n\n");

    // Store current dir containing glasscord.asar to copy it later
    strcpy(gcordPATH, _getcwd(NULL, 0));
    strcat(gcordPATH, "\\resources\\glasscord.asar");
    // Get dir of VS CODE
    strcpy(PATH, getenv("LOCALAPPDATA"));
    strcat(PATH, "\\Programs\\Microsoft VS Code\\resources\\app");

    // Check if VS CODE directory exists, else exit
    dir_check = PATH;
    struct stat stat_buff;

    if (stat(dir_check, &stat_buff) != 0 && !S_ISDIR(stat_buff.st_mode))
    {
        terminate("Visual Studio code directory could not be found\nPLease check your installation and retry.", 1);
    }

    // Change working dir
    if (_chdir(PATH) != 0)
    {
        terminate("Working directory could not be changed.", 1);
    }

    /* |=====================================================================|
                    Start of file operations in VS CODE dir */

    // Opening glasscord.asar files for read/writing in previous and current directory respectively
    gcordfp = fopen(gcordPATH, "rb");
    o_gcordfp = fopen("glasscord.asar", "r");
    // If glasscord.asar not found, copy it from gcordPATH
    if (o_gcordfp == NULL)
    {
        fclose(o_gcordfp);
        if ((o_gcordfp = fopen("glasscord.asar", "wb")) == NULL)
        {
            terminate("glasscord.asar file could not be created in the Visual Studio Code directory. Please close VS Code and retry.", 1);
        }
        if (gcordfp == NULL)
        {
            terminate("glasscord.asar not found in the executable's directory.", 1);
        }

        size_t bytes;
        while ((bytes = fread(currentBUFF, 1, BUFFER_SIZE, gcordfp)) != 0)
        {
            if (fwrite(currentBUFF, 1, bytes, o_gcordfp) != bytes)
            {
                terminate("glasscord.asar could not be copied.", 1);
            }
        }
    }
    else
    {
        printf("glasscord.asar already present in VS Code directory.\n");
    }
    fclose(gcordfp);
    fclose(o_gcordfp);

    // Check if file package.original.json and package.json already exist. If package.json exists as well, check if
    // it has the required modification already, if not then just rename it and proceed
    if ((file_check = fopen("package.original.json", "r")) != NULL)
    {
        fclose(file_check);
        file_check = fopen("package.json", "r");
        while (!feof(file_check))
        {
            fgets(currentBUFF, BUFFER_SIZE, file_check);
            if (strstr(currentBUFF, "glasscord.asar") != NULL)
            {
                terminate("Required modifications already present in package.json file.", 0);
            }
        }
        fclose(file_check);
        if (remove("package.original.json") == -1)
        {
            terminate("Unable to remove the existing package.original.json file.", 1);
        }
    }
    else if ((file_check = fopen("package.json", "r")) == NULL)
    {
        terminate("Cannot open package.json. Either the file doesn't exist or you don't have the right privilieges.", 1);
    }
    fclose(file_check);
    if (rename("package.json", "package.original.json") == -1)
    {
        terminate("Unable to rename package.json to package.original.json.", 1);
    }

    // Copy data from package.original.json to package.json and modifies package.json
    ifp = fopen("package.original.json", "r");
    ofp = fopen("package.json", "w+");
    if (ifp == NULL || ofp == NULL)
    {
        terminate("Cannot open/create package.original.json/package.json file respectively.", 1);
    }
    int is_mod = 0;
    while (!feof(ifp))
    {
        fgets(currentBUFF, BUFFER_SIZE, ifp);
        if (is_mod == 0 && strstr(currentBUFF, searchSTR) != NULL)
        {
            fputs(asarSTR, ofp);
            is_mod = 1;
        }
        else
        {
            fputs(currentBUFF, ofp);
        }
    }
    fclose(ifp);
    fclose(ofp);
    /*                End of file operations in VS CODE dir
      |=====================================================================| */

    terminate(NULL, 0);
    return 0;
}

//  +-+-+-+-+-+-+-+-+-+-+-+-
//  Display message and exit
//  +-+-+-+-+-+-+-+-+-+-+-+-
void terminate(char *msgStr, int exitCode)
{
    if(exitCode != 0 && msgStr != NULL)
    {
        printf("Error: %s", msgStr);
        spinner(3);
    }
    else if(exitCode == 0 && msgStr == NULL)
    {
        printf("Glasscord was successfully installed/reinstalled\n");
    }
    else if(exitCode == 0)
    {
        printf("%s\n", msgStr);
    }
    else
    {
        printf("An error ocurred but no information was provided");
        spinner(3);
    }
    printf("Press ENTER to exit...");
    getchar();
    exit(exitCode);
}

//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Spinner animation w/ added delay in seconds
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void spinner(int time_sec)
{
    int counter, i = 0;
    
    printf(" ");
    while (i < 3)
    {
        for (counter = 0; counter < 4; counter++)
        {
            printf("\b%c", "|/-\\"[counter]);
            fflush(stdout);
            Sleep(250 * (time_sec / 3.0));
        }
        i++;
    }
    printf("\b ");
    printf("\n");
}