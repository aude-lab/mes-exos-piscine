#include "priority_queue.hh"

size_t PriorityQueue::size() const
{
    return m_heap.size();
}

void PriorityQueue::push(std::unique_ptr<Node> node)
{
    m_heap.push_back(std::move(node));
    std::ranges::sort(m_heap, [](const std::unique_ptr<Node>& a,
                                  const std::unique_ptr<Node>& b)
    {
        return a->weight > b->weight;  
    });
}

std::unique_ptr<Node> PriorityQueue::pop()
{
    if (m_heap.empty())
        throw std::runtime_error("PriorityQueue::pop: queue is empty");

    auto node = std::move(m_heap.back());
    m_heap.pop_back();
    return node;
}
