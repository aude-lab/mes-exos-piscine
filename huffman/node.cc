#include "node.hh"

Node::Node(float weight_, std::optional<char> symbol_)
    : weight(weight_)
    , symbol(symbol_)
    , left(nullptr)
    , right(nullptr)
{}

Node::Node(float weight_, std::unique_ptr<Node> left_,
           std::unique_ptr<Node> right_)
    : weight(weight_)
    , symbol(std::nullopt)  
    , left(std::move(left_))
    , right(std::move(right_))
{}
