#include "WordTree.hpp"

#include <algorithm> // For std::transform and std::all_of
#include <cctype>    // For std::tolower and std::isalpha
#include <queue>

/**
 * Constructor: WordTree
 * Initializes an empty word tree with a root node.
 */
WordTree::WordTree() :
    root(std::make_shared<TreeNode>()) {}

/**
 * Function: add
 * Adds a word to the word tree by iterating through each character,
 * creating new nodes where needed, and marking the end of the word.
 *
 * Parameters:
 *   word - The word to be added to the tree.
 */
void WordTree::add(std::string word)
{
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

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

/**
 * Function: find
 * Searches for a given word in the word tree, returning true if the
 * word exists, or false otherwise. The search is case-insensitive.
 *
 * Parameters:
 *   word - The word to search for in the tree.
 *
 * Returns:
 *   True if the word is found, false otherwise.
 */
bool WordTree::find(std::string word)
{
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    bool isAlpha = std::all_of(word.begin(), word.end(), [](unsigned char c)
                               {
                                   return std::isalpha(c);
                               });

    if (word.empty() || !isAlpha)
    {
        return false;
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

/**
 * Function: size
 * Calculates the total number of words stored in the word tree.
 * Uses breadth-first search (BFS) to traverse all nodes and count words.
 *
 * Returns:
 *   The number of words in the tree.
 */
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

/**
 * Function: predict
 * Predicts possible word completions for a given partial string. It
 * traverses the tree to the end of the partial string, then performs a
 * breadth-first search to gather predictions up to the specified limit.
 *
 * Parameters:
 *   partial - The starting string for generating predictions.
 *   howMany - The maximum number of predictions to return.
 *
 * Returns:
 *   A vector of predicted words that match the given partial.
 */
std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    std::transform(partial.begin(), partial.end(), partial.begin(), [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    bool isAlpha = std::all_of(partial.begin(), partial.end(), [](unsigned char c)
                               {
                                   return std::isalpha(c);
                               });

    if (partial.empty() || !isAlpha)
    {
        return {};
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

        if (node->endOfWord && prefix.size() > partial.size())
        {
            predictions.push_back(prefix);
            if (predictions.size() >= howMany)
                break;
        }

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
