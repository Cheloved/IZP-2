#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// ----- Define new data types ----- //

// Universe, set, relation, calculation
enum DataType{ U, S, R, C };

// For better readability
typedef char* Element;

/*
    Struct for relation's elements.

    Since universe contains all possible elements,
    instead of working with char arrays, it's
    easyer and faster to use ints, so
    (leftElement rightElement) = (universe[leftIndex] universe[rightIndex])
*/
typedef struct RelationEl_s RelationElement;
struct RelationEl_s
{
    Element leftElement;
    Element rightElement;
    int leftIndex;
    int rightIndex;
};

/*
    Struct for data lines

    Same thing as above.
    elements[i] = universe[ indexes[i] ]
    ^ char        ^ char    ^ int
*/
typedef struct Line_s Line;
struct Line_s
{
    int                         id; // Number of row + 1 ( universe.id = 1, etc)
    enum DataType            _type; // U/S/R/C for universe, set, relation and calculation

    int               elementCount; // Number of elements in line(does not include type)
    Element*              elements; // Array of elements (*char)
    int*                   indexes; // Array of indexes of elements in universe

    RelationElement**    relations; // Pointer to an array of relation elements
}; 
// --------------------------------- //

// ----- Functions working with strings ----- //
// Gets length of string
int strLen(char* str)
{
    int i = 0;
    while ( str[i] != 0 )
        i++;

    return i;
}

// Compares two strings. Return 1 if they're equal
int strCmp(char* str1, char* str2)
{
    int len1 = strLen(str1);
    int len2 = strLen(str2);

    if ( len1 != len2 )
        return 0;
    
    for ( int i = 0; i < len1; i++ )
        if ( str1[i] != str2[i] )
            return 0;

    return 1;
}
// ------------------------------------------ //

// ----- Functions working with set's/relation's elements ----- //
// Compares two elements
// ( added for better readability )
int isEqual( Element e1, Element e2 )
{
    return strCmp((char*) e1, (char*) e2);
}

// Checks wheather number exists in int array
int isIn ( int element, int* array, int arraySize )
{
    for ( int i = 0; i < arraySize; i++ )
        if ( element == array[i] )
            return 1;
    
    return 0;
}

// Find index of string in array if it exists in there.
// If not, return -1;
int indexOfStr(char* str, char** array, int arraySize)
{
    for( int i = 0; i < arraySize; i++ )
    {
        if ( isEqual(str, array[i]) )
            return i;
    }
    return -1;
}

// Find index of element in data line.
// If element is not in data line, return -1;
int indexOf(Element e, Line* line)
{
    for ( int i = 0; i < line->elementCount; i++ )
        if ( isEqual(e, line->elements[i]) )
            return i;
    return -1;
}

// This function works faster than the one above, 
// because comparing strings takes more steps, that
// comparing numbers.
int indexByNum(int idx, Line* line)
{
    for ( int i = 0; i < line->elementCount; i++ )
        if ( idx == line->indexes[i] )
            return i;
    return -1;
}

// Get index of relation element in relation data line.
// If not exists, return -1
int indexOfRelationByNum(int left, int right, Line relation)
{
    for ( int i = 0; i < relation.elementCount; i++ )
        if ( left == relation.relations[i]->leftIndex )
            if ( right == relation.relations[i]->rightIndex )
                return i;
    
    return -1;
}
// ------------------------------------------------------------ //

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
    for( int i = 0; i < universe.elementCount; i++ )         // Go through all elements in universe.
    {                                                        // If element does not exists in this set,
        if ( indexByNum(universe.indexes[i], &line) == -1 )  // print it
            printf("%s ", universe.elements[i]);
    }
    printf("\n");
}

// Prints union of two sets
void _union(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )         // Print all the elements
        printf("%s ", lineA.elements[i]);                  // in first set
    
    for ( int i = 0; i < lineB.elementCount; i++ )         // Than check, wheather elements from B
        if ( indexByNum(lineB.indexes[i], &lineA) == -1 )  // exist in A.
            printf("%s ", lineB.elements[i]);              // If not, print it
    
    printf("\n");
}

// Prints intersection of two sets
void intersect(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )         // If elements exists in A
        if ( indexByNum(lineA.indexes[i], &lineB) != -1 )  // AND in B,
            printf("%s ", lineA.elements[i]);              // print it
    printf("\n");
}

// Prints A \ B
void minus(Line lineA, Line lineB)
{
    for ( int i = 0; i < lineA.elementCount; i++ )         // If element exists in A,
        if ( indexByNum(lineA.indexes[i], &lineB) == -1 )  // but does not exist in B,
            printf("%s ", lineA.elements[i]);              // Print it
    printf("\n");
}

// Checks, wheather set A is subset of B
void subseteq(Line lineA, Line lineB)
{
    // Number of elements in B should be greater
    // than in A
    if ( lineB.elementCount < lineA.elementCount )
    {
        printf("false\n");
        return;
    }

    // All elements in A should exist in B.
    // If at least one does not, print "false"
    for( int i = 0; i < lineA.elementCount; i++ )
        if ( indexByNum(lineA.indexes[i], &lineB) == -1 )
        {
            printf("false\n");
            return;
        }

    printf("true\n");
}

// Checks if A is proper subset of B
void subset(Line lineA, Line lineB)
{
    // Number of elements in A should be neither
    // greater nor equal(in this case they could be equal)
    if ( lineB.elementCount <= lineA.elementCount )
    {
        printf("false\n");
        return;
    }

    // All elements in A should exist in B.
    // If at least one does not, print "false"
    for( int i = 0; i < lineA.elementCount; i++ )
        if ( indexByNum(lineA.indexes[i], &lineB) == -1 )
        {
            printf("false\n");
            return;
        }
    
    printf("true\n");
}

// Checks if A equals B
void equals(Line lineA, Line lineB)
{
    // Cardinality should be equal
    if ( lineA.elementCount != lineB.elementCount )
    {
        printf("false");
        return;
    }

    // If at least one element is not equal,
    // print "false"
    for ( int i = 0; i < lineA.elementCount; i++ )
        if ( indexByNum(lineA.indexes[i], &lineB) == -1 )
        {
            printf("false\n");
            return;
        }

    printf("true");
}
// --------------------------------------- //

// ----- Functions working with relations ----- //
// Checks if relation is reflexive
int reflexive(Line relation, Line universe)
{
    // All of universe elements should be in relation
    // with themselves ( aRa ).
    // If at least one is not, print "false"
    for ( int i = 0; i < universe.elementCount; i++ )
        if ( indexOfRelationByNum(i, i, relation) == -1 )
        {
            printf("false\n");
            return 0;
        }
    
    printf("true\n");
    return 1;
}

// Checks if relation is symmetric
int symmetric(Line relation)
{
    for ( int i = 0; i < relation.elementCount; i++ )
    {
        int left = relation.relations[i]->leftIndex;
        int right = relation.relations[i]->rightIndex;              // If (left, right) is in relation,
        if ( indexOfRelationByNum( right, left, relation ) == -1 )  // but (right, left) is not,
        {                                                           // relations is not symmetric. return "false"
            printf("false\n");
            return 0;
        }
    }
    printf("true\n");
    return 1;
}

// Check if relation if antisymmetric
int antisymmetric(Line relation)
{
    for ( int i = 0; i < relation.elementCount; i++ )
    {
        int left = relation.relations[i]->leftIndex;
        int right = relation.relations[i]->rightIndex;              // If (left, right) is in relation
        if ( indexOfRelationByNum( right, left, relation ) != -1 )  // and (right, left) is in relation
        {                                                           // 'left' should be equal to 'right'
            if ( left != right )
            {
                printf("false\n");
                return 0;
            }
        }
    }
    printf("true\n");
    return 1;
}

// Checks if relation is transitive
int transitive(Line relation)
{
    for ( int i = 0; i < relation.elementCount; i++ )
    {
        int a = relation.relations[i]->leftIndex;                   // If aRb
        int b = relation.relations[i]->rightIndex;                  // Look for such 'c', that
        for ( int c = 0; c < relation.elementCount; c++ )           // (b, c) is in relation.
            if ( indexOfRelationByNum(b, c, relation) != -1 )       // If found, (a, c) must be in relation.
                if ( indexOfRelationByNum(a, c, relation) == -1 )   // If not, print "false"
                {
                    printf("false\n");
                    return 0;
                }
    }
    printf("true\n");
    return 1;
}

// Checks if relation is function
int function(Line relation)
{
    // Each 'left' element should have only ONE 'right'.
    // If it has more, 'right's should be equal.
    // If not, print "false"
    for ( int i = 0; i < relation.elementCount - 1; i++ )
    {
        int left = relation.relations[i]->leftIndex;
        int right = relation.relations[i]->rightIndex;
        for ( int j = i + 1; j < relation.elementCount; j++ )
            if ( relation.relations[j]->leftIndex == left )
                if ( relation.relations[j]->rightIndex != right )
                {
                    printf("false\n");
                    return 0;
                }
    }
    printf("true\n");
    return 1;
}

// Print domain of relation
int domain(Line relation)
{
    // Allocate memory for domain
    int maxLen = relation.elementCount;
    int* d = (int*)calloc(maxLen, sizeof(int));
    
    // And initialize them as -1's
    for ( int i = 0; i < maxLen; i++ )
        d[i] = -1;

    for ( int i = 0; i < maxLen; i++ )
        if ( !isIn(relation.relations[i]->leftIndex, d, maxLen) )  // If 'left' is not in domain yet
        {
            d[i] = relation.relations[i]->leftIndex;               // Add them to array
            printf("%s ", relation.relations[i]->leftElement);     // and print to stdout
        }
    printf("\n");
    return 0;
}

// Prints codomain of relation
int codomain(Line relation)
{
    // Works on the same principle, as function above
    int maxLen = relation.elementCount;
    int* d = (int*)calloc(maxLen, sizeof(int));
    for ( int i = 0; i < maxLen; i++ )
        d[i] = -1;

    for ( int i = 0; i < maxLen; i++ )
        if ( !isIn(relation.relations[i]->rightIndex, d, maxLen) )
        {
            d[i] = relation.relations[i]->rightIndex;
            printf("%s ", relation.relations[i]->rightElement);
        }
    printf("\n");
    return 0;
}

// Checks if relations is injective
int injective(Line relation, int print)
{
    // If it's called from bijective function
    // print == 0
    for ( int i = 0; i < relation.elementCount - 1; i++ )
    {
        int left = relation.relations[i]->leftIndex;
        int right = relation.relations[i]->rightIndex;
        for ( int j = i + 1; j < relation.elementCount; j++ )
            if ( relation.relations[j]->rightIndex == right )   // If for the same right parts
                if ( relation.relations[j]->leftIndex != left ) // there're different left
                {                                               // relation is not injective
                    if ( print )
                        printf("false\n");
                    return 0;
                }
    }
    if ( print )
        printf("true\n");
    return 1;
}

// Checks if relation is surjective
int surjective(Line relation, Line universe, int print)
{
    // If it's called from bijective function
    // print == 0
    for ( int i = 0; i < universe.elementCount; i++ )                       // Each element in universe
    {                                                                       // should have at least one
        int argExists = 0;                                                  // 'left' in relation
        for ( int j = 0; j < relation.elementCount; j++ )
        {
            if ( relation.relations[j]->rightIndex == universe.indexes[i] )
            {
                argExists = 1;
                break;
            }
        }
        if ( argExists )
            continue;

        if ( print )
            printf("false\n");
        return 0;
    }
    if ( print )
        printf("true\n");
    return 1;
}

// Checks if relation is bijective
// (should be either injective AND surjective)
int bijective(Line relation, Line universe)
{
    if ( injective(relation, 0) && surjective(relation, universe, 0) )
        printf("true\n");
    else
        printf("false\n");
    
    return 0;
}
// -------------------------------------------- //

// Reads one line from file
int readLine(char** line, FILE* file, long fileSize)
{
    char c;                   // Buffer character
    int len = 0;              // Length of the line
    int lastLine = 0;         // Whether it's the last line in file or not
    long begin = ftell(file); // Current position in file

    // Find length of the line first
    while ( (c = getc(file)) != '\n' )
    {
        len++;
        // Catch end of file
        if ( ftell(file) + 1 >= fileSize )
        {
            lastLine = 1;
            break;
        }
    }
    // If line is empty, stop reading
    if ( len == 0 )
        return 1;

    // Reset read position
    fseek(file, begin, SEEK_SET);

    // Allocate memory for characters
    *line = malloc(len * sizeof(char) + sizeof(char));

    // Read line char by char
    for ( int i = 0; i < len; i++ )
        *(*line + i) = getc(file);

    // Add end of string
    *(*line + len) = 0;

    // Skip '\n' character
    fseek(file, 1, SEEK_CUR);

    // Stop reading if it was last line
    if ( lastLine )
        return 1;

    return 0;
}

// Get elements count according to data type
int getElementCount(char* line, enum DataType _type)
{
    // Get character, by which string will be splitted
    char splitChar;
    switch (_type) {
        case R:  splitChar = '('; break;
        default: splitChar = ' '; break;
    }

    // Get number of that characters
    int count = 0;
    int len = strLen(line);
    for ( int i = 0; i < len; i++ )
        if ( line[i] == splitChar )
            count++;

    return count;
}

// Find first space in string after 'begin' character
int findSpaceAfter(int begin, char* str)
{
    int index = begin;
    while ( str[index] != ' ' )
    {
        index++;
        if ( str[index] == 0 )  // Later we'll use index of 
            return index;       // end-of-line, so return it if there's no spaces
    }
    return index;
}

// Splits string into elements
int splitElements(Line* line, char* str)
{
    // Get split char according to type of data
    char splitChar;
    switch (line->_type) {
        case R:  splitChar = '('; break;
        default: splitChar = ' '; break;
    }

    // Allocate memory for elements
    line->elements = calloc( line->elementCount, sizeof(char*) );

    int count = 0;         // Number of elements that was readed
    int len = strLen(str); // Length of string

    // Find index of first element (first space + 1)
    int start = 0; 
    while ( str[start] != splitChar )
        start++;
    start++;

    // This loop goes though the string
    // char by char
    for ( int i = start; i < len; i++ )
    {
        // If current character equeals split char
        // or it's the last character in line,
        // get element and write it
        if ( (str[i] == splitChar) || (i == (len - 1)) )
        {
            // Get index of an end of the element
            int end = i - 1;

            // Without this condition, last element will lose last char
            if ( i == len - 1 )
                    end++;

            // Allocate memory for element
            line->elements[count] = calloc( (end - start + 2), sizeof(char) );

            // Number of characters that was readed
            int charCount = 0; 

            // Go from start index to end index
            for ( int j = start; j <= end; j++ )
            {
                // Skip right bracket 
                if ( (line->_type == R) && (str[j] == ')') )
                    break;

                line->elements[count][charCount] = str[j];    // Set character 
                charCount++;                                  // Increase number
            }

            line->elements[count][charCount] = 0;             // Set end-of-string
            count++;                                          // Increase number
            start = i + 1;                                    // Move start index
        }
    }
    // Rewrite element count
    line->elementCount = count;

    return 0;
}

// Subdivide element into two different variables
int subdivideRelations(Line* line)
{
    // Allocate memory for relations array
    line->relations = (RelationElement**)calloc(line->elementCount, sizeof(RelationElement**));

    // Go through all of the elements
    for ( int i = 0; i < line->elementCount; i++ )
    {
        // Find space (which is split symbol)
        int spaceIndex = findSpaceAfter(0, line->elements[i]);

        // Get length of the element string
        int len = strLen( line->elements[i] );

        // Allocate memory for single relation
        line->relations[i] = (RelationElement*)calloc(1, sizeof(RelationElement*));

        // Allocate memory for element strings 
        line->relations[i]->leftElement = calloc(spaceIndex + 1, sizeof(char));
        line->relations[i]->rightElement = calloc(len - spaceIndex, sizeof(char));

        // Copy data to left element
        for ( int j = 0; j < spaceIndex; j++ )
            line->relations[i]->leftElement[j] = line->elements[i][j];

        // Copy data to right element
        for ( int j = spaceIndex + 1; j < len; j++ )
            line->relations[i]->rightElement[ j - spaceIndex - 1] = line->elements[i][j];
    }
    return 0;
}

// Reads line from a file and
// passes data from it to the data structure
int getData(Line* lineBuffer, FILE* file, long fileSize, int id)
{
    // Read a line of data and handle a return value
    char* line;
    int e = readLine(&line, file, fileSize);
    if ( e == -1 )  // Handle error
        return -1;

    // Print line
    printf("\n%s\n", line);

    // Get data type according
    // to the first symbor in line
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

    // Get number of elements
    lineBuffer->elementCount = getElementCount(line, lineBuffer->_type);

    // Split elements
    if ( splitElements(lineBuffer, line) != 0 )
       return -1;

    // Also split elements into relation elements
    // if type is relation
    if ( _type == R )
        if ( subdivideRelations(lineBuffer) != 0 )
            return -1;

    // If it was the last line, stop reading
    if ( e == 1 )
        return 1;
    return 0;
}

// Returns number of chars in file
long getFileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);   // Go to the end of file
    int fileSize = ftell(file); // Get position
    fseek(file, 0, SEEK_SET);   // Reset position to 0
    return fileSize;
}

// Assign indexes to the universe
// it'll be just (0, 1, 2, ..., n), where
// n is number of elements
int assingnUniverse(Line* line)
{
    line->indexes = calloc(line->elementCount, sizeof(int));
    for ( int i = 0; i < line->elementCount; i++ )
        line->indexes[i] = i;
    return 0;
}

// Assign indexes to a data line
// according to the universe.
// E.g. universe = {a, b, c, d};
// set = { b, d }; function will assign {1, 3}
int assignElementIndexes(Line* line, Line* universe)
{
    // Do not assign elements to 
    // calculation data line
    if ( line->_type == C )
        return 0;

    // If it's relation
    if ( line->_type == R )
    {
        // Set indexes both for left and right elements
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
        line->indexes = calloc(line->elementCount, sizeof(int)); // Allocate memory for the array
        for ( int i = 0; i < line->elementCount; i++ )           // of indexes
        {
            int idx = indexOf(line->elements[i], universe);      // Get index from universe
            if ( idx == -1 )
                return -1;
            else                                                 // If it exists, print.
                line->indexes[i] = idx;                          // else return error
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
    void(*setFunctionsUnar[2])(Line line) = {&empty, &card};

    char* setFunctionsBinarNames[7] = {"complement", "union", "intersect", "minus", "subseteq", "subset", "equals"};
    void(*setFunctionsBinar[7])(Line lineA, Line lineB) = {&complement, &_union, &intersect, &minus, &subseteq, &subset, &equals};

    char* relFunctionsNames_1arg[6] = { "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain" };
    int(*relFunctions_1arg[6])(Line line) = { &symmetric, &antisymmetric, &transitive, &function, &domain, &codomain };

    char* relFunctionsNames_2arg[2] = { "reflexive", "bijective" };
    int(*relFunctions_2arg[2])(Line line, Line universe) = { &reflexive, &bijective };

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

        // If line contains calculation
        if ( lines[totalLines]._type == C )
        {
            // Define index of calc function
            int funcIdx = -1;

            // Get command
            char* command = lines[totalLines].elements[0];

            // Get arguments
            int commandIdx1 = atoi( lines[totalLines].elements[1] );

            int commandIdx2 = 1;
            // If there's more than 1 argument
            if ( lines[totalLines].elementCount > 2 )
                commandIdx2 = atoi( lines[totalLines].elements[2] );

            // Check if calling unar operation
            if ( (funcIdx = indexOfStr(command, setFunctionsUnarNames, 2)) != -1 )
                setFunctionsUnar[funcIdx]( lines[commandIdx1 - 1] ); 

            // Check if calling binary operation
            if ( (funcIdx = indexOfStr(command, setFunctionsBinarNames, 7)) != -1 )
                setFunctionsBinar[funcIdx]( lines[commandIdx1 - 1], lines[commandIdx2 - 1] ); 

            if ( (funcIdx = indexOfStr(command, relFunctionsNames_1arg, 6)) != -1 )
                relFunctions_1arg[funcIdx]( lines[commandIdx1 - 1] );

            if ( (funcIdx = indexOfStr(command, relFunctionsNames_2arg, 2)) != -1 )
                relFunctions_2arg[funcIdx]( lines[commandIdx1 - 1], lines[0] );

            if ( strCmp(command, "injective") )
                injective(lines[commandIdx1 - 1], 1);

            if ( strCmp(command, "surjective") )
                surjective(lines[commandIdx1 - 1], lines[0], 1);
        }

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