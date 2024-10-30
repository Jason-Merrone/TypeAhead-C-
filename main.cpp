#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

std::shared_ptr<WordTree> readDictionary(std::string filename);

void displayPredictions(const std::vector<std::string>& predictions)
{
    rlutil::locate(1, 3); // Display predictions starting from line 3
    std::cout << "--- prediction ---\n";
    for (const auto& prediction : predictions)
    {
        std::cout << prediction << "\n";
    }
}

// Extracts the current word being typed
std::string getCurrentWord(const std::string& input)
{
    size_t pos = input.find_last_of(" ");
    return (pos == std::string::npos) ? input : input.substr(pos + 1);
}

int main()
{
    std::shared_ptr<WordTree> wordTree = readDictionary("dictionary.txt");
    std::string input;
    char ch;
    rlutil::cls();
    rlutil::locate(1, 1);
    rlutil::hidecursor();

    while (true)
    {
        ch = getch();

        if (ch == rlutil::KEY_ENTER)
        {
            break;
        }
        else if (ch == rlutil::KEY_BACKSPACE && !input.empty())
        {
            input.pop_back();
        }
        else if (ch >= 32 && ch <= 126)
        {
            input.push_back(ch);
        }

        rlutil::cls();
        rlutil::locate(1, 1);
        std::cout << input;

        // Get the current word and fetch predictions based only on it
        std::string currentWord = getCurrentWord(input);
        auto predictions = wordTree->predict(currentWord, 10);
        displayPredictions(predictions);
    }

    rlutil::showcursor();
    return 0;
}

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        {
                            return std::isalpha(c);
                        }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           {
                               return static_cast<char>(std::tolower(c));
                           });
            wordTree->add(word);
        }
    }

    return wordTree;
}
