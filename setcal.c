#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Universe, set, relation, calculation
enum DataType{ U, S, R, C };

typedef char* Element;


typedef struct RelationEl_s RelationElement;
struct RelationEl_s
{
    Element leftElement;
    Element RightElement;
};

typedef struct Line_s Line;
struct Line_s
{
    enum DataType        _type;
    int                     id;
    Element*          elements; 
    RelationElement* relations;
    int           elementCount;
}; 

int strLen(char* str)
{
    int i = 0;
    while ( str[i] != 0 )
        i++;

    return i;
}

int readLine(char** line, FILE* file, long fileSize)
{
    char c;            // Buffer character
    int len = 0;       // Find length of the line
    int lastLine = 0;  // Whether it's the last line in file or not
    long begin = ftell(file);

    while ( (c = getc(file)) != '\n' )
    {
        if ( ftell(file) + 1 >= fileSize )
            lastLine = 1;
        len++;
    }
    if ( len == 0 )
        return 1;

    // Reset read pointer
    fseek(file, begin, SEEK_SET);

    // Allocate memory
    *line = malloc(len * sizeof(char) + sizeof(char));
    // Read line
    for ( int i = 0; i < len; i++ )
        *(*line + i) = getc(file);

    // Add end of string
    *(*line + len) = 0;

    // Skip '\n' character
    fseek(file, 1, SEEK_CUR);

    if ( lastLine )
        return 1;

    return 0;
}

int getElementCount(char* line, enum DataType _type)
{
    char splitChar;
    switch (_type) {
        case R:  splitChar = '('; break;
        default: splitChar = ' '; break;
    }
    int count = 0;
    int len = strLen(line);
    for ( int i = 1; i < len; i++ )
        if ( line[i] == splitChar )
            count++;

    return count;
}

int splitElements(Line* line, char* str)
{
    char splitChar;
    switch (line->_type) {
        case R:  splitChar = '('; break;
        default: splitChar = ' '; break;
    }
    line->elements = malloc(line->elementCount * sizeof(char*));
    int count = 0;
    int len = strLen(str);

    // Find index of first element (first space + 1)
    int start = 0;
    while ( str[start] != splitChar )
        start++;
    start++;

    for ( int i = start; i < len; i++ )
    {
        if ( (str[i] == splitChar) || (i == (len - 1)) )
        {
            int end = i - 1;
            if ( i == len - 1 )
                end++;
            
            if ( end - start == 0 )
                continue;

            Element elementBuf = calloc( (end - start + 1), sizeof(char));
            int charCount = 0;
            for ( int j = start; j <= end; j++ )
            {
                elementBuf[charCount] = str[j];
                charCount++;
            }
            elementBuf[charCount] = '\0';
            //printf("%s\n",elementBuf);
            line->elements[count] = elementBuf;
            count++;
            start = i + 1;
        }
    }
    line->elementCount = count;

    return 0;
}

int getData(Line* lineBuffer, FILE* file, long fileSize, int id)
{
    // Read a line of data
    char* line;
    int e = readLine(&line, file, fileSize);  // Return value placeholder
    if ( e == -1 )  // Handle error
        return -1;
    printf("%s\n", line);

    /* Get data type */
    enum DataType _type = U;
    switch (line[0]) 
    {
        case 'U': { _type = U; break; }
        case 'S': { _type = S; break; }
        case 'R': { _type = R; break; }
        case 'C': { _type = C; break; }
        default: { fprintf(stderr, "Error: unknown identifier\n"); return -1; }
    }

    lineBuffer->_type = _type;
    lineBuffer->id    = id;
    if ( splitElements(lineBuffer, line) != 0 )
       return -1;

    if ( e == 1 )
        return 1;
    return 0;
}

long getFileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    return fileSize;
}

int readFile(char* path)
{
    FILE* file;
    if ( (file = fopen(path, "rt")) == NULL )
    {
        fprintf(stderr, "Error: cannot open file\n");
        return -1; 
    }

    //  Get number of characters in file
    long fileSize = getFileSize(file);

    Line lines[1000];
    int  totalLines = 0;

    Line lineBuffer;

    int e = 0; // Return value placeholder
    while ( 1 )
    {
        e = getData(&lineBuffer, file, fileSize, totalLines);
        if ( e == -1 )
            return -1;
        
        lines[totalLines] = lineBuffer;
        printf("  Id: %d\n  Type: %d\n  Element count: %d\n", lines[totalLines].id, lines[totalLines]._type, lines[totalLines].elementCount);        
        printf("  Elements:\n");

        for ( int i = 0; i < lines[totalLines].elementCount; i++ )
            printf("    Element %d: %s\n", i, lines[totalLines].elements[i]);

        totalLines++;

        if ( e == 1 )
            break;
    }

    return 0;
}

int readArgs(int argc, char* argv[], char** path)
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

    strcpy( *path, argv[1] );
    return 0;
}

int main(int argc, char* argv[])
{
    char* path;    
    if ( readArgs(argc, argv, &path) != 0 )
        return -1;

    if ( readFile(path) != 0 )
        return -1;
        
    return 0;
}