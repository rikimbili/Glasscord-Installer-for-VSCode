/*
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

int vscodeInstall(void);
int menu(void);
void updateCSS(FILE *fptr, int preset);
void terminate(char *msgStr, int exitCode);
void spinner(unsigned int time_sec);

int main(void)
{
    /* |===========================|
          Title and Menu system 
    */
    printf("\t  ______ _                                     _    _____                      _ _             \n");
    printf("\t / _____) |                                   | |  (_   _)           _        | | |            \n");
    printf("\t| /  ___| | ____  ___  ___  ____ ___   ____ _ | |    | |  ____   ___| |_  ____| | | ____  ____ \n");
    printf("\t| | (___) |/ _  |/___)/___)/ ___) _ \\ / ___) || |    | | |  _ \\ /___)  _)/ _  | | |/ _  )/ ___)\n");
    printf("\t| \\____/| ( ( | |___ |___ ( (__| |_| | |  ( (_| |   _| |_| | | |___ | |_( ( | | | ( (/ /| |    \n");
    printf("\t \\_____/|_|\\_||_(___/(___/ \\____)___/|_|   \\____|  (_____)_| |_(___/ \\___)_||_|_|_|\\____)_|    \n\n");
    printf("\nSelect a program for Glasscord Installer to check and install required components:\n");

    while(1)
    {
        switch(menu())
        {
            case 0:
                exit(0);
                break;
            case 1:
                if(vscodeInstall())
                {
                    printf("--------------------------------------------------------------\n");
                    printf("-> Glasscord for VS Code installed successfully!\n");
                }
                break;
        }
        printf("--------------------------------------------------------------\n");
        printf("Do you wish to install Glasscord to any other program or exit? \n");
    }

    return 0;
}

//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Prints the home screen's specified module. 0 for title and 1 for option menu
int menu(void)
{
    int user_choice, counter = 0;
      
    printf("\n1. Visual Studio Code\n0. Exit\n\n-> ");
    scanf("%d", &user_choice);
    while(user_choice != 0 && user_choice != 1)
    {
        if (counter == 4)
            printf("We can keep doing this forever if you want.\n");
        else if (counter == 5)
            printf("There are only two choices, I'm wondering where you got %d from.\n", user_choice);
        else if (counter == 6)
            printf("Okay, I'm starting to think your cat sat on your keyboard or something.\n");
        else if(counter >= 10)
        {
            printf("Alright, I'm gonna do you a favor and press 0 for you...");
            spinner(3);
            exit(0);
        }
        printf("-> ");
        scanf("%d", &user_choice);
        counter++;
    }
    return user_choice;
}

//  +-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Perform all required operations to install glasscord for VS Code, 1 returned if successful
int vscodeInstall(void)
{
    FILE *file_check;
    char currentBUFF[BUFFER_SIZE];
    char PATH[PATH_SIZE], executablePATH[PATH_SIZE],gcordPATH[PATH_SIZE];
    const char searchSTR[] = {"\"main\":"}, asarSTR[] = {"  \"main\": \"./glasscord.asar\",\n"};

    // Store current dir containing glasscord.asar to copy it later
    strcpy(executablePATH, _getcwd(NULL, 0));
    strcpy(gcordPATH, executablePATH);
    strcat(gcordPATH, "\\resources\\glasscord.asar");

    // Get dir of VS CODE
    strcpy(PATH, getenv("LOCALAPPDATA"));
    strcat(PATH, "\\Programs\\Microsoft VS Code\\resources\\app");

    // Check if VS CODE directory exists, else exit
    struct stat stat_buff;
    if (stat(PATH, &stat_buff) != 0 && !S_ISDIR(stat_buff.st_mode))
    {
        terminate("Visual Studio code directory could not be found\nPLease check your installation and retry.", -1);
        return 0;
    }
    // Change working dir to Microsoft VS Code/resources/app
        if (_chdir(PATH) != 0)
    {
        terminate("Unable to change working directory.", -1);
        return 0;
    }

    // Opening glasscord.asar files for read/writing, If glasscord.asar not found, copy it from gcordPATH
    FILE *gcordfp, *o_gcordfp;
    gcordfp = fopen(gcordPATH, "rb");
    o_gcordfp = fopen("glasscord.asar", "r");
    if (o_gcordfp == NULL)
    {
        fclose(o_gcordfp);
        if ((o_gcordfp = fopen("glasscord.asar", "wb")) == NULL)
        {
            terminate("glasscord.asar file could not be created in the Visual Studio Code directory. Please close VS Code and retry.", -1);
            return 0;
        }
        if (gcordfp == NULL)
        {
            terminate("glasscord.asar not found in the executable's directory.", -1);
            return 0;
        }

        size_t bytes;
        while ((bytes = fread(currentBUFF, 1, BUFFER_SIZE, gcordfp)) != 0)
        {
            if (fwrite(currentBUFF, 1, bytes, o_gcordfp) != bytes)
            {
                terminate("glasscord.asar could not be copied.", -1);
                return 0;
            }
        }
    }
    else
    {
        // printf("glasscord.asar already present in VS Code directory.\n");
    }
    fclose(gcordfp);
    fclose(o_gcordfp);

    // Check if file package.original.json and package.json already exist. If package.json exists as well, check if
    // it has the required modification already, if not then just rename it and proceed
    int is_package_mod = 0;
    if ((file_check = fopen("package.original.json", "r")) != NULL)
    {
        fclose(file_check);
        file_check = fopen("package.json", "r");
        while (!feof(file_check))
        {
            fgets(currentBUFF, BUFFER_SIZE, file_check);
            if (strstr(currentBUFF, "glasscord.asar") != NULL)
            {
                // printf("Required modifications already present in package.json file.\n");
                is_package_mod = 1;
            }
        }
        fclose(file_check);
        if (!is_package_mod && remove("package.original.json") == -1)
        {
            terminate("Unable to remove the existing package.original.json file.", -1);
            return 0;
        }
    }
    else if ((file_check = fopen("package.json", "r")) == NULL)
    {
        terminate("Cannot open package.json. Either the file doesn't exist or you don't have the right privilieges.", -1);
        return 0;
    }
    fclose(file_check);
    if (!is_package_mod && rename("package.json", "package.original.json") == -1)
    {
        terminate("Unable to rename package.json to package.original.json.", -1);
        return 0;
    }


    if(!is_package_mod)
    {
        // Copy data from package.original.json to package.json and modifies package.json
        FILE *ifp, *ofp;
        ifp = fopen("package.original.json", "r");
        ofp = fopen("package.json", "w+");
        if (ifp == NULL || ofp == NULL)
        {
            terminate("A file cannot be opened in VS Code directory. Please close VS Code and retry.", -1);
            return 0;
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
    }
    /*                End of file operations in VS CODE dir
      |=====================================================================| 
    */

    /*                Start of file operations in user/.vscode dir
      |=====================================================================| 
    */
    // Directory check and change
    FILE *fptr_css;
    int css_modified = 0, preset;

    strcpy(PATH, "C:\\Users\\");
    strcat(PATH, getenv("USERNAME"));
    strcat(PATH, "\\.vscode");

    if (_chdir(PATH) != 0)
    {
        terminate("Unable to change directory to .vscode for css file verification.", 1);
    }
    if ((fptr_css = fopen("glasscord_theme.css", "r")) == NULL)
    {
        terminate("glasscord_theme.css cannot be found in .vscode directory.\nType 0 to create file with default values, or type 1 for my customized css file: ", -1);
        
        
    }
    else{
        while(!feof(fptr_css))
        {
            fgets(currentBUFF, BUFFER_SIZE, fptr_css);
            if(strstr(currentBUFF, "glasscord") == 0)
            {
                css_modified = 1;
                break;
            }
        }
        if(!css_modified)
        {
            printf("The css file doesnt have the required glasscord elements.\n Type 0 to create file with default values, or type 1 for my customized css file: ");
            scanf("%d", &preset);
            createCSS(fptr_css, preset);
        }
    }
    




   // Change dir back to executable's path
    if (_chdir(executablePATH) != 0)
    {
        terminate("Unable to return to the executable's directory.", 1);
    }
    return 1;
}

//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  create glasscord_theme.css with given preset code. 0 = default, 1 = custom
void createCSS(FILE *fptr, int preset)
{
    fptr = fopen("glasscord_theme.css", "w");
    switch(preset)
    {
        case 1:
            
            break;

        default:
            break;
            
            
    }
}

//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Display message and exit the program, -1 exitCode used when a program termination is not desired
void terminate(char *msgStr, int exitCode)
{
    if(exitCode && msgStr)
    {
        printf("Error: %s", msgStr);
        spinner(3);
    }
    else if(exitCode == 0 && msgStr == NULL)
    {
        printf("No errors reported during ins\n");
    }
    else if(exitCode == 0)
    {
        printf("%s\nGlasscord was successfully installed\n", msgStr);
    }
    else
    {
        printf("An error ocurred but no information was provided");
        spinner(3);
    }

    if(exitCode == -1)
    {
        printf("Press ENTER to continue...");
        getchar();
    }
    else
    {
        printf("Press ENTER to exit...");
        getchar();
        exit(exitCode);
    }
}

//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Spinner animation w/ added delay in seconds
void spinner(unsigned int time_sec)
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