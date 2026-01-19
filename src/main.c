#include <stdio.h>
#include <string.h>

const char maxfile_path[] = "res/Maxfile";

int process(char *buf, FILE *file)
{
    if (!strchr(buf, '\n') && !feof(file))
    {
        fprintf(stderr, "line too long\n");
        return 1;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return strlen(buf);
}

int main()
{
    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("error opening %s\n", maxfile_path); return 1; }

    char buf[1024];
    while (fgets(buf, sizeof(buf), maxfile))
    {
        int len = process(buf, maxfile);
        if (len == 1) { return 1; }

        // test { echo hello }
        // cool { echo cool }

        printf("%d\n", len);
    }

    fclose(maxfile);

    return 0;
}
