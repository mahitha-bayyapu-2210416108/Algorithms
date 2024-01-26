#ifndef _MY_EDIT_DISTANCE_H_
#define _MY_EDIT_DISTANCE_H_
#include <iostream>
#include <vector>
#include <string>
#include <numeric> // Include for std::iota
#include <algorithm> // Include for std::transform
/*------------------------------------------------------------------------------
EditDistance: find the minimum number of edits that converts one string to
another
str1: the first input string, has an alphabet of [A, C, G, T]
str2: the second input string, has an alphabet of [A, C, G, T]
operations: the list of edit operations as string:
[M] for match, [C] for convert, [I] for insert, [D] for delete
returns the number of edit distance
------------------------------------------------------------------------------*/
unsigned int minValRec(unsigned int x, unsigned int y)
{
    if (x > y)
    {
        return y;
    }
    else
    {
        return x;
    }
}
unsigned int minimumDistance(unsigned int x, unsigned int y, unsigned int z)
{
    return minValRec(x, minValRec(y, z));
}
unsigned int EditDistance(const std::string &inputStr1, const std::string
&inputStr2, std::string &sequence)
{
    int stringLen1 = inputStr1.length();
    int stringLen2 = inputStr2.length();
    std::vector<std::vector<unsigned int>> editDistanceTable(stringLen1 + 1,
    std::vector<unsigned int>(stringLen2 + 1, 0));
    for (int i = 0; i <= stringLen1; ++i)
    {
        for (int j = 0; j <= stringLen2; ++j)
        {
            if (i == 0)
                editDistanceTable[i][j] = j;
            else if (j == 0)
                editDistanceTable[i][j] = i;
            else if (inputStr1[i - 1] == inputStr2[j - 1])
                editDistanceTable[i][j] = editDistanceTable[i - 1][j - 1];
            else
                editDistanceTable[i][j] = 1 + minimumDistance(editDistanceTable[i][j - 1], editDistanceTable[i - 1][j], editDistanceTable[i - 1][j - 1]);
        }
    }
    int i = stringLen1, j = stringLen2;
    while (i > 0 || j > 0)
    {
        char operation = ' ';
        if (i > 0 && j > 0 && inputStr1[i - 1] == inputStr2[j - 1])
        {
            operation = 'M';
            i--;
            j--;
        }
        else if (i > 0 && j > 0 && editDistanceTable[i][j] == editDistanceTable[i -
        1][j - 1] + 1)
        {
            operation = 'C';
            i--;
            j--;
        }
        else if (j > 0 && editDistanceTable[i][j] == editDistanceTable[i][j - 1] +
        1)
        {
            operation = 'I';
            j--;
        }
        else if (i > 0 && editDistanceTable[i][j] == editDistanceTable[i - 1][j] +
        1)
        {
            operation = 'D';
            i--;
        }
        sequence = operation + sequence;
    }
    return editDistanceTable[stringLen1][stringLen2];
}
/*------------------------------------------------------------------------------
PrintAlgnment: prints the two aligned sequences and the corresponding
operations
in the alignment form to stdout
str1: the first input string, has an alphabet of [A, C, G, T]
str2: the second input string, has an alphabet of [A, C, G, T]
editOperations: the list of edit operations as string:
[M] for match, [C] for convert, [I] for insert, [D] for delete
The alignment output should contain three rows:
The first row corresponds to the first sequence with gaps
The second row corresponds to the list of operations
The third row corresponds to the second sequence with gaps
A match [M] operation should be represented using '|'
A convert [C] operation should be represented using '*'
Insert and delete operations should be represented using ' ' (empty space)
Example:
str1: "ACAACC"
str2: "CAAAAC"
editOperations: "DMMMICM"
ACAACC
-CAAAAC
output alignment:
ACAA-CC
||| *|
-CAAAAC
------------------------------------------------------------------------------*/
void PrintAlignment(
const std::string &str1,
const std::string &str2,
const std::string &editOperations)
{
    std::string alignedInputStr1 = "";
    std::string alignedLine = "";
    std::string alignedInputStr2 = "";
    int i = 0, j = 0;
    for (int k = 0; k < editOperations.length(); ++k)
    {
        if (editOperations[k] == 'M' || editOperations[k] == 'C')
        {
            alignedInputStr1 += str1[i];
            alignedInputStr2 += str2[j];
            alignedLine += (str1[i] == str2[j]) ? '|' : '*';
            ++i;
            ++j;
        }
        else if (editOperations[k] == 'I')
        {
            alignedInputStr1 += '-';
            alignedInputStr2 += str2[j];
            alignedLine += ' ';
            ++j;
        }
        else if (editOperations[k] == 'D')
        {
            alignedInputStr1 += str1[i];
            alignedInputStr2 += '-';
            alignedLine += ' ';
            ++i;
        }
    }
    for (; i < str1.length(); ++i)
    {
        alignedInputStr1 += str1[i];
        alignedInputStr2 += '-';
        alignedLine += ' ';
    }
    for (; j < str2.length(); ++j)
    {
        alignedInputStr1 += '-';
        alignedInputStr2 += str2[j];
        alignedLine += ' ';
    }
    std::cout << alignedInputStr1 << std::endl;
    std::cout << alignedLine << std::endl;
    std::cout << alignedInputStr2 << std::endl;
}
#endif