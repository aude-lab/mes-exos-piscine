#include "store/store_impl.hh"

#include "exceptions.hh"

namespace libzork::store
{

    const story::Node* StoreImpl::get_active_node() const
    {
        return active_node_;
    }

    void StoreImpl::set_active_node(const story::Node* node)
    {
        active_node_ = node;
    }

    bool StoreImpl::has_variable(const std::string& name) const
    {
        return variables_.find(name) != variables_.end();
    }

    int StoreImpl::get_variable(const std::string& name) const
    {
        auto it = variables_.find(name);
        return (it != variables_.end()) ? it->second : 0;
    }

    void StoreImpl::set_variable(const std::string& name, int value)
    {
        variables_[name] = value;
    }

    std::map<std::string, int> StoreImpl::get_inventory() const
    {
        std::map<std::string, int> inventory;

        for (const auto& [name, value] : variables_)
        {
            if (value > 0 && name.back() != '_')
            {
                inventory[name] = value;
            }
        }

        return inventory;
    }

} // namespace libzork::store
