#pragma once
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class TreeNode
{
  public:
    std::string value;
    bool endOfWord;
    std::unordered_map<char, std::shared_ptr<TreeNode>> children;
};

class WordTree
{
  public:
    WordTree();
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size();

  private:
    bool isAllLowercaseAlpha(const std::string& str);
    std::shared_ptr<TreeNode> root;
};