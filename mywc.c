/*
    2021-01-17 Batuhan Düzgün
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

#define VERSION_DESC    "@Copyright 2021 - Version 1.0 - MIT License Custom wc software module. Developed by Batuhan Düzgün\n"


#define HELP_DESC       "****** HELP DOCUMENT *******\n" \
                        "- Programın komut satırı argümanlarının genel yapısı şöyledir: \n" \
                        "                     \n" \
                        "wc [seçenekler] [dosya listesi] \n" \
                        "                     \n" \
                        "wc programı seçeneksiz argüman verilmediğinde stdin dosyasından okuma yapmaktadır. Programın komut satırı argümanları şöyledir:\n" \
                        "                     \n" \
                        "- c ya da --bytes: Yalnızca dosyadaki byte sayısını yazdırır. Yazdırma şöyle yapılmalıdır:\n" \
                        "<byte sayısı>  <dosya ismi>\n" \
                        "<byte sayısı>  <dosya ismi>\n" \
                        "<byte sayısı>  <dosya ismi>\n" \
                        "...\n" \
                        "                     \n" \
                        "-l ya da --lines: Dosyada kaç satır bulunduğunu yazdırır. Satır sayısında \n ile bitmeyen son satır sayılmaz. Başka bir deyişle wc programı sonu '\n' ile biten satırları saymaktadır. Yazdırma şöyle yapılmaktadır:\n" \
                        "<satır sayısı>     <dosya ismi>\n" \
                        "<satır sayısı>     <dosya ismi>\n" \
                        "<satır sayısı>     <dosya ismi>\n" \
                        "...\n" \
                        "                     \n" \
                        "-L ya da --max-line-length: Dosya içerisindeki en uzun satırın karakter uzunluğunu yazdırır. Yazdırma şöyle yapılmalıdır:\n" \
                        "<satırdaki karakter sayısı>    <dosya ismi>\n" \
                        "<satırdaki karakter sayısı>    <dosya ismi>\n" \
                        "<satırdaki karakter sayısı>    <dosya ismi>\n" \
                        "                     \n" \
                        "-w ya da --words: Dosyadaki sözcük sayısını yazdırır. \n" \
                        "-- help: Yardım yazısını yazdırır:\n" \
                        "-- version: Versiyon numarasını yazdırır.\n" \
                        "                     \n" \
                        "Eğer yukarıdaki seçeneklerden hiçbiri verilmezse program default olarak şöyle bir çıktı vermelidir:\n" \
                        "<satır sayısı> <sözcük sayısı> <byte sayısı> <dosya ismi>\n" \
                        "<satır sayısı> <sözcük sayısı> <byte sayısı> <dosya ismi>\n" \
                        "<satır sayısı> <sözcük sayısı> <byte sayısı> <dosya ismi>\n"


void parse_opts(int *argc, char *argv[], struct option *options, 
                int *result, int *error_flag, int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag);
void validity_check(int *error_flag, int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag);
void run(int *argc, char *argv[], int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag);
int open_file(FILE **file, char *file_name);
long find_line_count(FILE *file, char *file_name, long *max_line_length);
long find_word_count(FILE *file, char *file_name);
long find_character_count(FILE *file, char *file_name);
void read_from_stdin();

int main(int argc, char *argv[])
{   
    int result;
    int error_flag = 0;
    int c_flag, l_flag, L_flag, w_flag, help_flag, version_flag;

    struct option options[] = {
        { "bytes", no_argument, NULL, 'c' },
        { "lines", no_argument, NULL, 'l' },
        { "max-line-length", no_argument, NULL, 'L' },
        { "words", no_argument, NULL, 'w' },
        { "help", no_argument, NULL, 1 },
        { "version", no_argument, NULL, 2 },
        { 0, 0, 0, 0 }
    };

    opterr = 0;
    c_flag = l_flag = L_flag = w_flag = help_flag = version_flag = 0;

    parse_opts(&argc, argv, options, &result, &error_flag, &c_flag, &l_flag, &L_flag, &w_flag, &help_flag, &version_flag);

    validity_check(&error_flag, &c_flag, &l_flag, &L_flag, &w_flag, &help_flag, &version_flag);
    
    run(&argc, argv, &c_flag, &l_flag, &L_flag, &w_flag, &help_flag, &version_flag);

    return 0;
}

void run(int *argc, char *argv[], int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag)
{
    FILE *file = NULL;
    char *file_name;
    long max_line_length = 0;

    if(*help_flag)
    {
        printf("%s\n", HELP_DESC);
    }
    else if(*version_flag)
    {
        printf("%s\n", VERSION_DESC);
    }
    else
    {
        if( (*argc) == optind )
        {
            read_from_stdin();
            return;
        }

        int major_options_used = (*c_flag) + (*l_flag) + (*L_flag) + (*w_flag);
        if(major_options_used == 0)
        {
            *l_flag = *w_flag = *c_flag = 1;
        }

        for (int i = optind; i < *argc; i++)
        {
            file_name = argv[i];
            int opened_file = open_file(&file, file_name);
            if(opened_file != 0)
            {
                continue;
            }

            if(*l_flag)
            {
                if(*L_flag)
                {
                    max_line_length = 1;
                }
                
                long line_count = find_line_count(file, file_name, &max_line_length);
                printf("%ld ", line_count);
            }

            if(*w_flag)
            {
                long word_count = find_word_count(file, file_name);
                printf("%ld ", word_count);
            }

            if(*c_flag)
            {
                long character_count = find_character_count(file, file_name);
                printf("%ld ", character_count);
            }

            if(max_line_length > 0)
            {
                printf("%ld ", max_line_length);
            }

            if(!(*l_flag) && (*L_flag))
            {
                max_line_length = 1;
                find_line_count(file, file_name, &max_line_length);
                printf("%ld ", max_line_length);
            }

            printf(" %s \n", file_name);

            fclose(file);
        }
    }
}

void read_from_stdin()
{
    char ch;
    while(read(STDIN_FILENO, &ch, 1) > 0)
    {
        if(ch == '\n')
        {
            printf("%c",ch);
        }
    }
}

int open_file(FILE **file, char *file_name)
{
    if ((*file = fopen(file_name, "r")) == NULL) 
    {   
        fprintf(stderr, "file %s cannot open!..\n", file_name);
        return 1;
    }
    return 0;
}

void validity_check(int *error_flag, int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag)
{
    if(*error_flag)
    {
        exit(EXIT_FAILURE);
    }

    int major_options_used = (*c_flag) + (*l_flag) + (*L_flag) + (*w_flag);

    if((*help_flag) && major_options_used > 0)
    {
        fprintf(stderr, "--help cannot be used with another option...\n");
        exit(EXIT_FAILURE);   
    }

    if((*version_flag) && major_options_used > 0)
    {
        fprintf(stderr, "--version cannot be used with another option...\n");
        exit(EXIT_FAILURE);
    }
}

void parse_opts(int *argc, char *argv[], struct option *options, 
                int *result, int *error_flag, int *c_flag, int *l_flag, int *L_flag, int *w_flag, int *help_flag, int *version_flag)
{
    while( ( *result = getopt_long(*argc, argv, "clLw", options, NULL)) != -1 )
    {
        switch( (*result) )
        {
            case 'c':
                *c_flag = 1;
                break;
            case 'l':
                *l_flag = 1;
                break;
            case 'L':
                *L_flag = 1;
                break;
            case 'w':
                *w_flag = 1;
                break;
            case 1:
                *help_flag = 1;
                break;
            case 2:
                *version_flag = 1;
                break;
            case '?':
                if(optopt != 0)
                {
                    fprintf(stderr, "invalid option -%c\n", optopt);
                }
                else
                {
                    fprintf(stderr, "invalid long option!...\n");
                }
                *error_flag = 1;
                break;
        }
    }
}

long find_line_count(FILE *file, char *file_name, long *max_line_length)
{   
    int val;
    long line_count = 0;
    long pos = 0;
    long max_diff = 0;
    int max_line_calculation_flag = 0;

    if(*max_line_length == 1)
    {
        *max_line_length = 0;
        max_line_calculation_flag = 1;
    }

    if(file == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_SET);

    while( (val = fgetc(file)) != EOF)
    {
        if (ferror(file)) 
        {
            fprintf(stderr, "value cannot read in file %s \n", file_name);
            break;
        }

        if(val == '\n')
        {
            if(max_line_calculation_flag)
            {
                long cur_pos = ftell(file);
                long diff = cur_pos - pos;
                if(diff > max_diff)
                {
                    max_diff = diff;
                }
                pos = cur_pos;
            }
            line_count++;
        }
    }
    if(max_line_calculation_flag)
    {
        *max_line_length = max_diff;
    }
    return line_count;
}

long find_word_count(FILE *file, char *file_name)
{
    long word_count = 0;
    int skip = 0;
    int val;
    long p = 0;
    long p_prev = 0;

    if(file == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    p = ftell(file);

    while( (val = fgetc(file)) != EOF)
    {
        if (ferror(file)) 
        {
            fprintf(stderr, "value cannot read in file %s \n", file_name);
            break;
        }

        if((ispunct(val) || isspace(val)) && (p - p_prev > 0))
        {
            skip = 1;
            p_prev = p;
            word_count++;
        }

        if(!skip)
        {
            if(val == EOF && (p - p_prev > 0))
            {
                word_count++;
            }
        }
        else
        {
            while( (val = fgetc(file)) != EOF )
            {
                if(!(ispunct(val) || isspace(val)))
                {
                    skip = 0;
                    break;  
                }
            }
        }
        p = ftell(file);
    }
    
    return word_count;
}

long find_character_count(FILE *file, char *file_name)
{
    char val;
    long character_count = 0;

    if(file == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    while( (val = fgetc(file)) != EOF)
    {
        if (ferror(file)) 
        {
            fprintf(stderr, "value cannot read in file %s \n", file_name);
            break;
        }
        character_count++;
    }
    return character_count;
}
