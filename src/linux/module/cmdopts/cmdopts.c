#include <stdio.h>
#include <string.h>

#include "cmdopts.h"

#include "../version/version.h"
#include "../usage/usage.h"

int cmdopts(int argc, char *argv[], char **maxfile_path)
{
    for (int argi = 0; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "-h") == 0 || strcmp(argv[argi], "--help") == 0)
        {
            printf("%s", usage);
            return 3;
        }
        else if (strcmp(argv[argi], "-v") == 0 || strcmp(argv[argi], "--version") == 0)
        {
            printf("%s", version);
            return 2;
        }
        else if (strcmp(argv[argi], "-m") == 0 || strcmp(argv[argi], "--maxfile") == 0)
        {
            if (argi + 1 < argc)
            {
                *maxfile_path = argv[argi + 1];
            }
            else { printf("argument option missing\n"); return 1; }
        }
    }

    return 0;
}
