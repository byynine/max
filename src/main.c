#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char maxfile_path[] = "res/Maxfile";

int process_line(char *lbuf, FILE *file)
{
    if (!strchr(lbuf, '\n') && !feof(file)) return -1;
    return strlen(lbuf);
}

char (*tokenize(char *str, const char delim))[64]
{
    if (!str) return NULL;

    size_t max_toks = 32;
    size_t tok_size = 64;
    size_t toksi = 0;

    char (*toks)[tok_size] = malloc(max_toks * tok_size);
    if (!toks) return NULL;

    char buf[2048];
    size_t bufi = 0;

    size_t slen = strlen(str);
    if (slen <= 0) return NULL;

    for (size_t i = 0; i < slen; i++)
    {
        if (bufi >= sizeof(buf) - 1) {
            printf("bufi");
            break;
        }
        if (str[i] == delim)
        {
            if (toksi >= max_toks) {
                printf("bufi");
                break;
            }

            buf[bufi] = '\0';

            strncpy(toks[toksi], buf, sizeof(toks[toksi]) - 1);
            toks[toksi][sizeof(toks[toksi]) - 1] = '\0';
            toksi++;

            bufi = 0;

            continue;
        }
        buf[bufi++] = str[i];
    }
    if (bufi >= sizeof(buf) - 1 || toksi >= max_toks) return NULL;

    buf[bufi] = '\0';

    strncpy(toks[toksi], buf, sizeof(toks[toksi]) - 1);
    toks[toksi][sizeof(toks[toksi]) - 1] = '\0';

    bufi = 0;

    return toks;
}

int is_key(char *str, char *key)
{
    if (!str || !key) return -1;

    size_t slen = strlen(str);
    size_t klen = strlen(key);

    if (klen > slen) { return -2; }

    return 0;
}


int main()
{
    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("error opening %s\n", maxfile_path); return 1; }

    char lbuf[1024];
    int li = 1;

    char (*toks)[64] = tokenize("hhellohelllohellohellohllohellohellohllohellohellohllohellohellohllohellohellohllohellohellohlohellohellohellohellohellohellohellohellohellohellohelloello bgege cwe", ' ');

    printf("%s\n", toks[0]);
    printf("%s\n", toks[1]);
    printf("%s\n", toks[2]);

    free(toks);

    while (fgets(lbuf, sizeof(lbuf), maxfile))
    {
        int len = process_line(lbuf, maxfile);
        if (len == -1) { printf("line %d: line too long\n", li); return 1; }


        li++;
    }

    fclose(maxfile);

    return 0;
}
