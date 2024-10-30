#include "WordTree.hpp"

#include <iostream>

int main()
{
    std::shared_ptr<WordTree> tree = std::make_shared<WordTree>();
    std::shared_ptr<WordTree> wordTree = std::make_shared<WordTree>();

    tree->add("tree");
    tree->add("three");
    tree->add("free");
    tree->add("pizza");
    wordTree->add("acknowledging");
    wordTree->add("acorn");
    wordTree->add("acorns");
    wordTree->add("acoustic");

    const auto predictions = wordTree->predict("acorn", 2);

    std::cout << (tree->find("pizza") ? "Yes" : "No") << std::endl;
    std::cout << tree->size() << std::endl;

    std::vector<std::string> prediction = tree->predict("t", 2);
    for (std::string p : predictions)
    {
        std::cout << p << std::endl;
    }
}