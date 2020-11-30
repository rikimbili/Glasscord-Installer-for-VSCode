/* 
This is a glasscord installer/reinstaller for Visual Studio Code and Discord(not supported at the moment). 
The purpose of this program is to copy/create necessary files whenever you execute it in the case that a 
software update breaks your glasscord integration. It is also designed for new users that just want to get 
glasscord installed without the hassle of manually doing each thing.

+-+-+-+-+-+-+-+-+-+-+-> TODO: <-+-+-+-+-+-+-+-+-+-+-+
-Discord support and ability to choose desired program
-Separate redundant operations into functions
-Create .css file in the vs code user dir if not present
-Add VSCode/Discord installation checks
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

void spinner(short time_secs);

int main()
{
    FILE *ifp, *ofp, *gcordfp, *o_gcordfp, *file_check;

    char PATH[PATH_SIZE], gcordPATH[PATH_SIZE];
    char currentBUFF[BUFFER_SIZE];
    const char searchSTR[] = {"\"main\":"}, asarSTR[] = {"  \"main\": \"./glasscord.asar\",\n"}, *dir_check;

    // Store current dir containing glasscord.asar to copy it later
    strcpy(gcordPATH, _getcwd(NULL, 0));
    strcat(gcordPATH, "\\glasscord.asar");
    // Get dir of VS CODE
    strcpy(PATH, getenv("LOCALAPPDATA"));
    strcat(PATH, "\\Programs\\Microsoft VS Code\\resources\\app");

    // Check if VS CODE directory exists, else exit
    dir_check = PATH;
    struct stat stat_buff;

    if (stat(dir_check, &stat_buff) == 0 && S_ISDIR(stat_buff.st_mode))
    {
        printf("Visual Studio Code directory found");
        spinner(1);
    }
    else
    {
        printf("Visual Studio code directory could not be found\nPLease check your installation and retry");
        spinner(7);
        exit(1);
    }

    // Change working dir
    if (_chdir(PATH) != 0)
    {
        printf("Error while changing working directory");
        spinner(5);
        exit(1);
    }

    /* |=====================================================================|
                    Start of file operations in VS CODE dir */

    // Opening glasscord.asar files for read/writing in previous and current directory respectively
    gcordfp = fopen(gcordPATH, "rb");
    o_gcordfp = fopen("glasscord.asar", "r");
    // If file not found, copy it from gcordPATH
    if (o_gcordfp == NULL)
    {
        fclose(o_gcordfp);
        if ((o_gcordfp = fopen("glasscord.asar", "wb")) == NULL)
        {
            printf("glasscord.asar file could not be created in the Visual Studio Code directory\nPlease close VS Code and retry");
            spinner(5);
            exit(1);
        }
        if (gcordfp == NULL)
        {
            printf("glasscord.asar not found in [%s] directory", gcordPATH);
            spinner(5);
            exit(1);
        }
        // Copying glasscord.asar, byte reading/writing used because the source is not formatted
        size_t bytes;
        while ((bytes = fread(currentBUFF, 1, BUFFER_SIZE, gcordfp)) != 0)
        {
            if (fwrite(currentBUFF, 1, bytes, o_gcordfp) != bytes)
            {
                printf("Error copying glasscord.asar");
                spinner(5);
                exit(1);
            }
        }
    }
    else
    {
        printf("glasscord.asar already present in VS Code directory\nResuming");
        spinner(3);
    }
    fclose(gcordfp);
    fclose(o_gcordfp);

    // Check if file package.original.json and package.json already exist. If package.json exists as well, check if
    // it has the required modification already, if not then just rename it and proceed
    if ((file_check = fopen("package.original.json", "r")) != NULL)
    {
        fclose(file_check);
        // Checks if package.json already has the required changes
        file_check = fopen("package.json", "r");
        while (!feof(file_check))
        {
            fgets(currentBUFF, BUFFER_SIZE, file_check);
            if (strstr(currentBUFF, "glasscord.asar") != NULL)
            {
                printf("Glasscord already implemented in package.json file\nNo change required");
                spinner(3);
                exit(0);
            }
        }
        fclose(file_check);
        if (remove("package.original.json") == -1)
        {
            printf("Unable to remove the existing package.original.json file");
            spinner(4);
            exit(1);
        }
    }
    else if ((file_check = fopen("package.json", "r")) == NULL)
    {
        printf("Error opening package.json\nEither the file doesn't exist or there is a permission error");
        spinner(5);
        exit(1);
    }
    fclose(file_check);
    if (rename("package.json", "package.original.json") == -1)
    {
        printf("Unable to rename package.json to package.original.json");
        spinner(5);
        exit(1);
    }

    // Copy data from ifp to ofp
    ifp = fopen("package.original.json", "r");
    ofp = fopen("package.json", "w+");
    if (ifp == NULL || ofp == NULL)
    {
        printf("Cannot open/create package.original.json/package.json file respectively");
        spinner(5);
        exit(1);
    }
    // fgets/fputs used because it allows me to modify the line when currentBUFF matches searchSTR
    while (!feof(ifp))
    {
        fgets(currentBUFF, BUFFER_SIZE, ifp);
        // if ["main":] is found in current line of ifp, write asarSTR to the same line in ofp
        if (strstr(currentBUFF, searchSTR) != NULL)
            fputs(asarSTR, ofp);
        else
            fputs(currentBUFF, ofp);
    }
    fclose(ifp);
    fclose(ofp);

    /*                End of file operations in VS CODE dir
      |=====================================================================| */

    printf("Glasscord was successfully installed!\nClosing console");
    spinner(4);

    return 0;
}

// Closing dots animation, cause why not
void spinner(short time_secs)
{
    printf(" ");
    short counter, i = 0;
    while (i < 3)
    {
        for (counter = 0; counter < 6; counter++)
        {
            printf("\b%c", ".oOOo."[counter]);
            fflush(stdout);
            Sleep(166.7 * (time_secs / 3.0));
        }
        if (i != 2)
            printf(".");
        i++;
    }
    printf("\n");
}