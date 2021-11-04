#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Universe, set, relation, calculation
enum DataType{ U, S, R, C };

typedef char* Element;

// Struct for relations
// (leftElement, rightElement)
typedef struct RelationEl_s RelationElement;
struct RelationEl_s
{
    Element leftElement;
    Element rightElement;
    int leftIndex;
    int rightIndex;
};

// Struct for data lines
typedef struct Line_s Line;
struct Line_s
{
    enum DataType            _type;
    int                         id;
    Element*              elements; 
    int*                   indexes;
    RelationElement**    relations;
    int               elementCount;
}; 

int strLen(char* str)
{
    int i = 0;
    while ( str[i] != 0 )
        i++;

    return i;
}

int isEqual( Element e1, Element e2 )
{
    int len1 = strLen( (char*)e1);
    int len2 = strLen( (char*)e2);

    if ( len1 != len2 )
        return 0;
    
    for ( int i = 0; i < len1; i++ )
        if ( e1[i] != e2[i] )
            return 0;
    
    return 1;
}

// Returns index of element e in line
int indexOf(Element e, Line* line)
{
    for ( int i = 0; i < line->elementCount; i++ )
    {
        if ( isEqual(e, line->elements[i]) )
            return i;
    }
    return -1;
}

/* HAVE TO MAKE INDEX OF ELEMENT BY INDEX VALUE, NOT BY CHAR* */

// ----- Functions working with sets ----- //

// Checks wheather set is empty or not
void empty(Line line)
{
    if ( line.elementCount > 0 )
        printf("false\n");
    else
        printf("true\n");
}

// Prints number of elements in the set
void card(Line line)
{
    printf("%d\n", line.elementCount);
}

// Prints complement of set 
void complement(Line line, Line universe)
{
    for( int i = 0; i < universe.elementCount; i++ )
    {
        if ( indexOf(universe.elements[i], &line) == -1 )
            printf("%s ", universe.elements[i]);
    }
    printf("\n");
}

void _union(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )
        printf("%s ", lineA.elements[i]);
    
    for ( int i = 0; i < lineB.elementCount; i++ )
        if ( indexOf(lineB.elements[i], &lineA) == -1 )
            printf("%s ", lineB.elements[i]);
    
    printf("\n");
}

void intersect(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )
        if ( indexOf(lineA.elements[i], &lineB) != -1 )
            printf("%s ", lineA.elements[i]);
    printf("\n");
}

void minus(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )
        if ( indexOf(lineA.elements[i], &lineB) == -1 )
            printf("%s ", lineA.elements[i]);
    printf("\n");
}

void equals(Line lineA, Line lineB)
{
    if ( lineA.elementCount != lineB.elementCount )
    {
        printf("false");
        return;
    }
    int eqv = 1;
    for ( int i = 0; i < lineA.elementCount; i++ )
        if ( lineA.elements[i] != lineB.elements[i] ) { eqv = 0; break; }

    if ( eqv )
        printf("true");
    else
        printf("false");
}

void subseteq(Line lineA, Line lineB)
{
    // smth
}

// --------------------------------------- //


int readLine(char** line, FILE* file, long fileSize)
{
    char c;            // Buffer character
    int len = 0;       // Find length of the line
    int lastLine = 0;  // Whether it's the last line in file or not
    long begin = ftell(file);

    while ( (c = getc(file)) != '\n' )
    {
        len++;
        if ( ftell(file) + 1 >= fileSize )
        {
            lastLine = 1;
            break;
        }
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
    for ( int i = 0; i < len; i++ )
        if ( line[i] == splitChar )
            count++;

    return count;
}

int findSpaceAfter(int begin, char* str)
{
    int index = begin;
    while ( str[index] != ' ' )
    {
        index++;
        if ( str[index] == 0 )
            return index;
    }
    return index;
}

int splitElements(Line* line, char* str)
{
    char splitChar;
    switch (line->_type) {
        case R:  splitChar = '('; break;
        default: splitChar = ' '; break;
    }
    line->elements = calloc( line->elementCount, sizeof(char*) );
    int count = 0;
    int len = strLen(str);

    // Find index of first element (first space + 1)
    int start = 0;
    while ( str[start] != splitChar )
        start++;
    start++;

    Element elementBuf;
    for ( int i = start; i < len; i++ )
    {
        if ( (str[i] == splitChar) || (i == (len - 1)) )
        {
            int end = i - 1;
            if ( i == len - 1 )
                    end++;

            line->elements[count] = calloc( (end - start + 2), sizeof(char) );
            int charCount = 0;
            for ( int j = start; j <= end; j++ )
            {
                if ( (line->_type == R) && (str[j] == ')') )
                    break;
                line->elements[count][charCount] = str[j];
                charCount++;
            }

            line->elements[count][charCount] = 0;
            count++;
            start = i + 1;
        }
    }
    line->elementCount = count;

    return 0;
}

int subdivideRelations(Line* line)
{
    line->relations = (RelationElement**)calloc(line->elementCount, sizeof(RelationElement**));
    for ( int i = 0; i < line->elementCount; i++ )
    {
        int spaceIndex = findSpaceAfter(0, line->elements[i]);
        int len = strLen( line->elements[i] );
        line->relations[i] = (RelationElement*)calloc(1, sizeof(RelationElement*));
        line->relations[i]->leftElement = calloc(spaceIndex + 1, sizeof(char));
        line->relations[i]->rightElement = calloc(len - spaceIndex, sizeof(char));

        for ( int j = 0; j < spaceIndex; j++ )
            line->relations[i]->leftElement[j] = line->elements[i][j];
        line->relations[i]->leftElement[spaceIndex] = '\0';

        for ( int j = spaceIndex + 1; j < len; j++ )
            line->relations[i]->rightElement[ j - spaceIndex - 1] = line->elements[i][j];
    }
    return 0;
}

int getData(Line* lineBuffer, FILE* file, long fileSize, int id)
{
    // Read a line of data and handle a return value
    char* line;
    int e = readLine(&line, file, fileSize);
    if ( e == -1 )  // Handle error
        return -1;

    // Print line
    printf("\n%s\n", line);

    /* Get data type */
    enum DataType _type = U;
    switch (line[0]) 
    {
        case 'U': { _type = U; break; } // Universe
        case 'S': { _type = S; break; } // Set
        case 'R': { _type = R; break; } // Relation
        case 'C': { _type = C; break; } // Calculation
        default: { fprintf(stderr, "Error: unknown identifier\n"); return -1; }
    }

    // Set struct members
    lineBuffer->_type = _type;
    lineBuffer->id    = id + 1;

    // 
    lineBuffer->elementCount = getElementCount(line, lineBuffer->_type);
    if ( splitElements(lineBuffer, line) != 0 )
       return -1;

    if ( _type == R )
        if ( subdivideRelations(lineBuffer) != 0 )
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

int assingnUniverse(Line* line)
{
    line->indexes = calloc(line->elementCount, sizeof(int));
    for ( int i = 0; i < line->elementCount; i++ )
        line->indexes[i] = i;
    return 0;
}

int assignElementIndexes(Line* line, Line* universe)
{
    if ( line->_type == C )
        return 0;

    if ( line->_type == R )
    {
        for ( int i = 0; i < line->elementCount; i++ )
        {
            int idxLeft = indexOf(line->relations[i]->leftElement, universe);
            if (idxLeft == -1)
            {
                printf("Error: %s does not exists in universe\n", line->relations[i]->leftElement);
                return -1;
            }
            int idxRight = indexOf(line->relations[i]->rightElement, universe);
            if (idxRight == -1)
            {
                printf("Error: %s does not exists in universe\n", line->relations[i]->rightElement);
                return -1;
            }

            line->relations[i]->leftIndex = idxLeft;
            line->relations[i]->rightIndex = idxRight;
        }
    } else {
        line->indexes = calloc(line->elementCount, sizeof(int));
        for ( int i = 0; i < line->elementCount; i++ )
        {
            int idx = indexOf(line->elements[i], universe);
            if ( idx == -1 )
                return -1;
            else
                line->indexes[i] = idx;
        }
    }
    return 0;
}

void printDataLine(Line line)
{
    printf("Data line %d\n", line.id);
    switch (line._type) 
    {
        case U: printf("  Type: Universe\n"); break;
        case S: printf("  Type: Set\n"); break;
        case R: printf("  Type: Relation\n"); break;
        case C: printf("  Type: Calculation\n"); break;
    }
    printf("  Element count: %d\n", line.elementCount);
    switch (line._type) 
    {
        case R: {
            for ( int i = 0; i < line.elementCount; i++ )
            {
                printf("    Element %d: (%s, %s)", i, line.relations[i]->leftElement, line.relations[i]->rightElement);
                printf("; Indexes: (%d, %d)\n", line.relations[i]->leftIndex, line.relations[i]->rightIndex);
            }
            break;
        }    
        case S: {
            for ( int i = 0; i < line.elementCount; i++ )
                printf("    Element %d: %s; Index: %d\n", i, line.elements[i], line.indexes[i]);
            break;
        }
        default: {
            for ( int i = 0; i < line.elementCount; i++ )
                printf("    Element %d: %s\n", i, line.elements[i]);
            break;
        }
    }
}

int readFile(char* path)
{
    // Open file and check if it exists
    FILE* file;
    if ( (file = fopen(path, "rt")) == NULL )
    {
        fprintf(stderr, "Error: cannot open file\n");
        return -1; 
    }

    //  Get number of characters in file
    long fileSize = getFileSize(file);

    // According to terms
    // Max number of lines is 1000
    Line lines[1000];

    // Count of readed lines
    int  totalLines = 0;

    // Buffer for current line
    Line lineBuffer;

    char* setFunctionsUnarNames[2] = {"empty", "card"};
    char* setFunctionsBinarNames[7] = {"complement", "union", "intersect", "minus", "subseteq", "subset", "equals"};
    void(*setFunctionsUnar[2])(Line line) = {&empty, &card};
    void(*setFunctionsBinar)(Line lineA, Line lineB); // = {&complement, &union, &intersect, &minus, &subseteq, &subset, &equals}

    char* relFunctionNames[10] = { "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain", "injective", "surjective", "bijective" };

    // Return value placeholder
    int e = 0;     
    while ( 1 )
    {
        // Read data and handle an error
        e = getData(&lineBuffer, file, fileSize, totalLines);
        if ( e == -1 )
            return -1;
        
        // Set line
        lines[totalLines] = lineBuffer;

        // Assign numbers to elements, since working with numbers
        // is faster than working with strings
        if ( totalLines == 0 )
            assingnUniverse( &lines[totalLines] );
        else
            assignElementIndexes(&lines[totalLines], &lines[0]);

        printDataLine(lines[totalLines]);

        // Increase number of readed lines
        totalLines++;

        // If file has been readed till the end
        if ( e == 1 )
            break;
    }

    return 0;
}

int readArgs(int argc, char* argv[], char** path)
{
    // Handle wrong arguments
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

    // Copy file path from argument to variable
    strcpy( *path, argv[1] );
    return 0;
}

int main(int argc, char* argv[])
{
    // Declaration of file path
    char* path;    

    // Read arguments from command line
    if ( readArgs(argc, argv, &path) != 0 )
        return -1;

    // Read file
    if ( readFile(path) != 0 )
        return -1;
        
    return 0;
}