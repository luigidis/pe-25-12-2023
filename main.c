#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 20
#define MAX_ARRAY_LENGTH 200
#define BUFFSIZE 1024

typedef struct p
{
    char *fileNameIn;
    char *fileNameOut;
} Parameters;

typedef struct record
{
    char key[MAX_STRING_LENGTH];
    int length;
} Record;

void error(char *e);
Parameters readInput(int argc, char *argv[]);
void buildArray(char *fileName, Record arrayToBuild[MAX_ARRAY_LENGTH]);
void filter(Record array[MAX_ARRAY_LENGTH]);
void createSet(Record array[MAX_ARRAY_LENGTH]);
void printArray(Record array[MAX_ARRAY_LENGTH]);
void save(char *fileOutput, Record array[MAX_ARRAY_LENGTH]);
void read(char *fileName);

int main(int argc, char *argv[])
{

    printf("==========A==========\n");
    Parameters p = readInput(argc, argv);
    printf("==========B==========\n");

    Record arrayToBuild[MAX_ARRAY_LENGTH];
    buildArray(p.fileNameIn, arrayToBuild);
    printf("==========c==========\n");
    filter(arrayToBuild);
    printArray(arrayToBuild);

    printf("==========D==========\n");
    createSet(arrayToBuild);
    printf("==========E==========\n");
    printArray(arrayToBuild);
    printf("==========F==========\n");
    save(p.fileNameOut, arrayToBuild);
    printf("==========G==========\n");
    read(p.fileNameOut);
    return 0;
}

void error(char *e)
{
    fprintf(stderr, "[ERROR]: %s\n", e);
    exit(EXIT_FAILURE);
}

Parameters readInput(int argc, char *argv[])
{
    if (argc != 3)
        error("Number of Arguments not valid!");

    char *fileNameIn = strdup(argv[1]);
    char *fileNameOut = strdup(argv[2]);

    if (fileNameIn && strcmp(fileNameIn + (strlen(fileNameIn) - 4), ".txt"))
        error("Exetension file input not correct");

    if (fileNameOut && strcmp(fileNameOut + (strlen(fileNameOut) - 4), ".bin"))
        error("Exetension file output not correct");

    Parameters p = {fileNameIn, fileNameOut};
    return p;
}

void buildArray(char *fileName, Record arrayToBuild[MAX_ARRAY_LENGTH])
{
    FILE *f = fopen(fileName, "r");

    if (!f)
        error("Error opening file!");

    for (int k = 0; k < MAX_ARRAY_LENGTH; k++)
    {
        arrayToBuild[k].length = -1;
    }

    char line[MAX_STRING_LENGTH];
    int i = 0;

    while (fgets(line, MAX_STRING_LENGTH, f))
    {
        char *token = strtok(line, " ");
        while (token != NULL && i < MAX_ARRAY_LENGTH)
        {
            int len = strlen(token);
            if (len > 0)
            {
                strncpy(arrayToBuild[i].key, token, MAX_STRING_LENGTH - 1);
                arrayToBuild[i].key[MAX_STRING_LENGTH - 1] = '\0';
                arrayToBuild[i].length = strlen(line);
                i++;
            }
            token = strtok(NULL, " ");
        }

        if (i >= MAX_ARRAY_LENGTH)
            break;
    }

    fclose(f);
}

void filter(Record arrayToBuild[MAX_ARRAY_LENGTH])
{

    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        int k = 0;

        for (int j = 0; j < MAX_STRING_LENGTH && arrayToBuild[i].key[j] != '\0'; j++)
        {
            if (!ispunct((unsigned char)arrayToBuild[i].key[j]))
            {
                arrayToBuild[i].key[k] = arrayToBuild[i].key[j];
                k++;
            }
        }
        arrayToBuild[i].key[k] = '\0';
        arrayToBuild[i].length = strlen(arrayToBuild[i].key);
    }
}

// Funzione che eliminare i record duplicati dall'array createSet()
void createSet(Record array[MAX_ARRAY_LENGTH])
{
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (array[i].length == -1)
        {
            for (int j = i + 1; j < MAX_ARRAY_LENGTH; j++)
            {
                if (strcmp(array[i].key, array[j].key) == 0 && array[j].length != -1)
                {
                    array[j].length = -1;
                }
            }
        }
    }
}

// Funzione che stampa printArray()
void printArray(Record array[MAX_ARRAY_LENGTH])
{
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (array[i].length != -1)
        {
            printf("Stringa stampata su flusso standard: %s\t", array[i].key);
            printf("Lenght: %d\n", array[i].length);
        }
    }
}

// Funzione che salva i record in un file binario save()
void save(char *fileOutput, Record array[MAX_ARRAY_LENGTH])
{
    FILE *f = fopen(fileOutput, "w");
    char buffer[BUFFSIZE];

    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (array[i].length != -1)
        {
            int length = snprintf(buffer, sizeof(buffer), "Print Array() [%d], [word: %s], [length: %d]\n", i + 1, array[i].key, array[i].length);
            /*fwrite() richiede:
                Un puntatore ai dati da scrivere (buffer).
                La dimensione di ogni elemento da scrivere (sizeof(char)).
                Il numero di elementi da scrivere (length, la lunghezza della stringa generata).
                Il file in cui scrivere (f).*/
            fwrite(buffer, sizeof(char), length, f);
        }
    }

    fclose(f);
}

// Funzione che legge e stampa il contenuto del file binari read()
void read(char *fileName)
{
    FILE *f = fopen(fileName, "r");
    if (!f)
        error("Erorr opening file");

    char buffer[BUFFSIZE];
    while (fgets(buffer, MAX_STRING_LENGTH, f))
    {   
        //Trova la posizone di Print Array()
        char *spacePos = strstr(buffer, "Print Array()");
        printf("spacePos: %s\n", spacePos);

        if (spacePos != NULL)
        {
            //Sostituisci "Print Array() con read()"
            printf("read() %s", spacePos + strlen("Print Array()"));
        }
    }

    fclose(f);
}