#include <iostream>
using namespace::std;
#include <fstream>
#include <string>
#include <ctime>

// input all words from the file Dictionary.txt, and put them to the array "dictionary"
void readDictionary(string* dictionary, int& numDicWords);

// return true if and only if wordToCheck == dictionary[ i ] for some i
bool legal(string wordToCheck, string* dictionary, int numDicWords);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is equal to the length of wordToCheck, and
// 2. wordToCheck and w are different on exactly one character.
// Put these words into the array "result".
void spellCheck1(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one greater than the length of wordToCheck, and
// 2. wordToCheck is equal to the substring obtained by deleting a character from w.
// Put these words into the array "result".
void spellCheck2(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// Find every word w in the array "dictionary" which satisfies two conditions:
// 1. the length of w is one less than the length of wordToCheck, and
// 2. w is equal to the substring obtained by deleting a character from wordToCheck.
// Put these words into the array "result".
void spellCheck3(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult);

// find the words in the array "dictionary" whose length is equal to the last parameter "length",
// and puts these words into the array "words"
void findWords(string*& words, int& numWords, string* dictionary, int numDicWords, size_t length);

// output all words in the array "result" to the file Result.txt
void saveWords(string* result, int numResult);

int main()
{
    srand(1);

    int numDicWords = 0;
    string* dictionary = new string[40000];

    readDictionary(dictionary, numDicWords);

    string wordToCheck;
    int numResult = 0;
    string* result = new string[3000];
    for (int i = 0; i < 10; i++)
    {
        wordToCheck = dictionary[rand() + numDicWords - 32768]; // skip short words
        wordToCheck.erase(rand() % wordToCheck.size(), 1);

        if (legal(wordToCheck, dictionary, numDicWords))
            cout << wordToCheck << " is a legal english word.\n";
        else
        {
            cout << wordToCheck << endl;
            result[numResult++] = wordToCheck;

            spellCheck1(wordToCheck, dictionary, numDicWords, result, numResult);
            spellCheck2(wordToCheck, dictionary, numDicWords, result, numResult);
            spellCheck3(wordToCheck, dictionary, numDicWords, result, numResult);
            result[numResult++] = "";
        }
    }

    saveWords(result, numResult);

    delete[] dictionary;
    delete[] result;

    system("pause");
}

bool legal(string wordToCheck, string* dictionary, int numDicWords)
{
    for (int i = 0; i < numDicWords; i++)
        if (dictionary[i] == wordToCheck)
            return true;
    return false;
}

void readDictionary(string* dictionary, int& numDicWords)
{
    ifstream inFile("Dictionary.txt", ios::in);

    if (!inFile)
    {
        cout << "File could not be opened" << endl;
        exit(1);
    }

    while (inFile >> dictionary[numDicWords])
        numDicWords++;

    inFile.close();
}

void spellCheck1(string wordToCheck, string* dictionary,
    int numDicWords, string* result, int& numResult)
{
    size_t length = wordToCheck.size();
    int numWords = 0;
    string* words = 0;

    findWords(words, numWords, dictionary, numDicWords, length);

    string buffer1;
    string buffer2;

    buffer2 = wordToCheck;

    for (int i = 0; i < numWords; i++)
    {
        buffer1 = words[i];

        for (size_t j = 0; j < length; j++)
        {
            size_t counter = 0;

            for (size_t n = 0; n < length; n++)
            {
                if (n == j)
                    continue;
                if (buffer1[n] == wordToCheck[n])
                    counter++;
            }
            if (counter == length - 1)
            {
                result[numResult] = buffer1;
                numResult++;
            }
        }
    }

    delete[] words;
}

void spellCheck2(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult)
{
    size_t length = wordToCheck.size() + 1;
    int numWords = 0;
    string* words = 0;

    findWords(words, numWords, dictionary, numDicWords, length);

    string buffer;
    for (int i = 0; i < numWords; i++)
    {
        buffer = words[i];

        for (size_t j = 0; j < length; j++)
        {
            size_t counter = 0;

            for (size_t n = 0; n < length; n++)
            {
                if (n == j)
                    continue;
                if (n < j)
                    if (buffer[n] == wordToCheck[n])
                        counter++;
                if(n > j)
                    if(buffer[n] == wordToCheck[n - 1])
                        counter++;
            }
            if (counter == length - 1)
            {
                result[numResult] = buffer;
                numResult++;
            }
        }
    }

    delete[] words;
}

void spellCheck3(string wordToCheck, string* dictionary, int numDicWords, string* result, int& numResult)
{
    size_t length = wordToCheck.size() - 1;
    int numWords = 0;
    string* words = 0;

    findWords(words, numWords, dictionary, numDicWords, length);

    string buffer;
    for (int i = 0; i < numWords; i++)
    {
        buffer = words[i];

        for (size_t j = 0; j < length + 1; j++)
        {
            size_t counter = 0;

            for (size_t n = 0; n < length + 1; n++)
            {
                if (n == j)
                    continue;
                if (n < j)
                    if (buffer[n] == wordToCheck[n])
                        counter++;
                if (n > j)
                    if (buffer[n - 1] == wordToCheck[n])
                        counter++;
            }
            if (counter == length)
            {
                result[numResult] = buffer;
                numResult++;
            }
        }
    }

    delete[] words;
}

void findWords(string*& words, int& numWords, string* dictionary,
    int numDicWords, size_t length)
{
    for (int i = 0; i < numDicWords; i++)
        if (dictionary[i].size() == length)
            numWords++;

    words = new string[numWords];

    numWords = 0;
    for (int i = 0; i < numDicWords; i++)
        if (dictionary[i].size() == length)
            words[numWords++] = dictionary[i];
}

void saveWords(string* result, int numResult)
{
    ofstream outFile("result.txt", ios::out);

    if (!outFile)
    {
        cout << "File could not be opened" << endl;
        exit(1);
    }

    for (size_t i = 0; i < numResult; i++)
        outFile << result[i] << endl;

    outFile.close();
}