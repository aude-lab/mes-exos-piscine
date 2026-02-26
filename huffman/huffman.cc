#include "huffman.hh"

#include <map>
#include <string>
#include <unordered_map>

std::unique_ptr<Node> buildHuffmanTree(const std::string& message)
{
    if (message.empty())
        return nullptr;

    std::map<char, int> freq;
    for (char c : message)
        freq[c]++;

    PriorityQueue queue;
    for (auto& [c, count] : freq)
        queue.push(std::make_unique<Node>(static_cast<float>(count), c));

    while (queue.size() > 1)
    {
        auto left = queue.pop();
        auto right = queue.pop();

        float parent_weight = left->weight + right->weight;

        queue.push(std::make_unique<Node>(parent_weight,
                                          std::move(left),
                                          std::move(right)));
    }

    return queue.pop();
}

static void buildCodeTableHelper(const std::unique_ptr<Node>& node,
                                  const std::string& prefix,
                                  std::unordered_map<char, std::string>& table)
{
    if (!node)
        return;

    if (node->symbol.has_value())
    {
        table[node->symbol.value()] = prefix.empty() ? "0" : prefix;
        return;
    }

    buildCodeTableHelper(node->left,  prefix + '0', table);
    buildCodeTableHelper(node->right, prefix + '1', table);
}

std::unordered_map<char, std::string>
buildCodeTable(const std::unique_ptr<Node>& node)
{
    std::unordered_map<char, std::string> table;
    buildCodeTableHelper(node, "", table);
    return table;
}