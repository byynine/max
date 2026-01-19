#include <stdio.h>
#include <string.h>

const char maxfile_path[] = "res/Maxfile";

int process(char *lbuf, FILE *file)
{
    if (!strchr(lbuf, '\n') && !feof(file))
    {
        printf("line too long\n");
        return -1;
    }
    return strlen(lbuf);
}

int main()
{
    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("error opening %s\n", maxfile_path); return 1; }

    char lbuf[1024];
    while (fgets(lbuf, sizeof(lbuf), maxfile))
    {
        int len = process(lbuf, maxfile);
        if (len == -1) { return 1; }

        for (int i = 0; i < len; i++)
        {
            char c = lbuf[i];
        }

        // var variable mycoolvalue
        // ref test
        // cmd echo variable
    }

    fclose(maxfile);

    return 0;
}
