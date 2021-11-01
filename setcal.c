#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Universe, set, relation, calculation
enum DataType{ U, S, R, C };

typedef struct Line_s Line;
struct Line_s
{
    enum DataType _type;
    int              id;
    char*  elements[31];
    int    elementCount;
}; 

int strLen(char* str)
{
    int i = 0;
    while ( str[i] != '\n' )
        i++;

    return i;
}

int readLine(char* line, FILE* file)
{
    char c;
    int len = 0;
    line = (char*)malloc(0);
    while ( (c = getc(file)) != '\n' )
    {
        len++;
        line = (char*)realloc(line, len * sizeof(char));
        *(line + len - 1) = c;
    }
    line = (char*)realloc(line, (len+1) * sizeof(char));
    *(line + len - 1) = '\0';
    return 0;
}

int getData(Line* lineBuffer, FILE* file, int id)
{
    char* line = "";
    readLine(line, file);

    enum DataType _type;

    /* Get data type */
    char t = getc(file);
    if ( t == 'U' )
        _type = U;
    else if ( t == 'S' )
        _type = S;
    else if ( t == 'R' )
        _type = R;
    else if ( t == 'C' )
        _type = C;
    lineBuffer->_type = _type;
    lineBuffer->id    = id;

    int elementCount = 0;
    char buffer[31] = "";

    return 0;
}

int readFile(char* path)
{
    FILE* file;

    printf("Path: %s\n", path);
    if ( (file = fopen(path, "rt")) == NULL )
    {
        fprintf(stderr, "Error: cannot open file");
        return -1; 
    }

    Line lines[1000];
    int  totalLines = 0;

    Line lineBuffer;
    getData(&lineBuffer, file, totalLines);
    //while ( getData(&lineBuffer, file, totalLines) == 0 )
    //{
        
   //}

    return 0;
}

int readArgs(int argc, char* argv[], char* path)
{
    if ( argc < 2 )
    {
        fprintf(stderr, "Error: not enough arguments\n");
        return -1;
    }
    if ( argc > 2 )
    {
        fprintf(stderr, "Error: too many arguments\n");
        return -1;
    }
    strcpy( path, argv[1] );
    return 0;
}

int main(int argc, char* argv[])
{
    char path[] = "";    
    if ( readArgs(argc, argv, path) != 0 )
        return -1;

    if ( readFile(path) != 0 )
        return -1;
        
    return 0;
}