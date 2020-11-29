    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       Inneficient unused code that checks the new file 
       for ["main":] and writes asarDIR to it 
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      
    rewind(ofp);
    while (!feof(ofp))
    {
        int i;

        fgets(CurrentSTR, BUFFER_SIZE, ofp);
        printf("%s\n", CurrentSTR);

        if (strstr(CurrentSTR, SearchSTR) != NULL)
        {
            printf("Found %s %ld\n", CurrentSTR, ftell(ofp));

            char c = getc(ofp);
            do
            {
                if (c == '\n')
                    fseek(ofp, -3, SEEK_CUR);
                else
                    fseek(ofp, -2, SEEK_CUR);
                c = getc(ofp);
                putchar(c);
            } while (c != ':');
            printf("\n");

            break;
        }
    }
    fprintf(ofp, "%s", asarDIR);
    */