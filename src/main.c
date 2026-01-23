#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char maxfile_path[] = "res/Maxfile";

int process_line(char *lbuf, FILE *file)
{
    if (!strchr(lbuf, '\n') && !feof(file)) return -1;
    return strlen(lbuf);
}

int iskey(char *buf, char *key)
{
    if (!buf || !key) return 0;

    size_t blen = strlen(buf);
    size_t klen = strlen(key);

    char *nbuf = malloc(1024);
    if (!nbuf) return 0;

    size_t nbufi = 0;

    for (size_t i = 0; i < blen; i++)
    {
        char c = buf[i];
        for (size_t j = 0; j < klen; j++)
        {
            if (nbufi >= klen) break;
            if (c == key[j]) nbuf[nbufi++] = key[j];
        }
    }

    nbuf[nbufi] = '\0';

    if (!strcmp(nbuf, key)) return 1;

    free(nbuf);

    return 0;
}

int main()
{
    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("error opening %s\n", maxfile_path); return 1; }

    char lbuf[1024];
    int li = 1;

    while (fgets(lbuf, sizeof(lbuf), maxfile))
    {
        int len = process_line(lbuf, maxfile);
        if (len == -1) { printf("line too long\n"); return 1; }

        if (iskey(lbuf, "ref "))
            printf("ref key on line %d\n", li);

        li++;
    }

    fclose(maxfile);

    return 0;
}
