#include "WordTree.hpp"

#include <algorithm> // For std::transform and std::all_of
#include <cctype>    // For std::tolower and std::isalpha
#include <queue>

WordTree::WordTree() :
    root(std::make_shared<TreeNode>()) {}

void WordTree::add(std::string word)
{
    // Convert word to lowercase
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    // Check if all characters are alphabetic
    bool isAlpha = std::all_of(word.begin(), word.end(), [](unsigned char c)
                               {
                                   return std::isalpha(c);
                               });

    if (word.empty() || !isAlpha)
    {
        return;
    }

    std::shared_ptr<TreeNode> currentNode = root;
    for (char c : word)
    {
        if (currentNode->children.find(c) == currentNode->children.end())
        {
            auto newNode = std::make_shared<TreeNode>();
            newNode->value = std::string(1, c);
            currentNode->children[c] = newNode;
        }
        currentNode = currentNode->children[c];
    }
    currentNode->endOfWord = true;
}

bool WordTree::find(std::string word)
{
    // Convert word to lowercase
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    // Check if all characters are alphabetic
    bool isAlpha = std::all_of(word.begin(), word.end(), [](unsigned char c)
                               {
                                   return std::isalpha(c);
                               });

    if (word.empty() || !isAlpha)
    {
        return false; // Return false if the word is empty or contains non-alphabetic characters
    }

    std::shared_ptr<TreeNode> currentNode = root;
    for (char c : word)
    {
        if (currentNode->children.find(c) == currentNode->children.end())
        {
            return false;
        }
        currentNode = currentNode->children[c];
    }
    return currentNode->endOfWord;
}

std::size_t WordTree::size()
{
    if (root == nullptr)
    {
        return 0;
    }

    std::size_t numOfWords = 0;
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(root);

    while (!q.empty())
    {
        std::shared_ptr<TreeNode> current = q.front();
        q.pop();

        if (current->endOfWord)
        {
            numOfWords++;
        }

        for (auto& [key, child] : current->children)
        {
            q.push(child);
        }
    }
    return numOfWords;
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    // Convert partial to lowercase
    std::transform(partial.begin(), partial.end(), partial.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    // Check if all characters are alphabetic
    bool isAlpha = std::all_of(partial.begin(), partial.end(), [](unsigned char c)
                               {
                                   return std::isalpha(c);
                               });

    if (partial.empty() || !isAlpha)
    {
        return {}; // Return an empty vector if partial is empty or contains non-alphabetic characters
    }

    std::vector<std::string> predictions;
    std::shared_ptr<TreeNode> currentNode = root;

    // Traverse the tree to the end of the partial string
    for (char c : partial)
    {
        if (currentNode->children.find(c) == currentNode->children.end())
        {
            return predictions;
        }
        currentNode = currentNode->children[c];
    }

    // BFS to find predictions
    std::queue<std::pair<std::shared_ptr<TreeNode>, std::string>> q;
    q.push({ currentNode, partial });

    while (!q.empty() && predictions.size() < howMany)
    {
        auto [node, prefix] = q.front();
        q.pop();

        // Only add predictions that extend beyond the partial word
        if (node->endOfWord && prefix.size() > partial.size())
        {
            predictions.push_back(prefix);
            if (predictions.size() >= howMany)
                break;
        }

        // Gotta sort the children to pass the test i guess
        std::vector<std::pair<char, std::shared_ptr<TreeNode>>> children(
            node->children.begin(), node->children.end());
        std::sort(children.begin(), children.end(),
                  [](const auto& a, const auto& b)
                  {
                      return a.first < b.first;
                  });

        for (const auto& [ch, child] : children)
        {
            q.push({ child, prefix + ch });
        }
    }
    return predictions;
}
