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
        (void)name;
        throw NotImplemented();
    }

    int StoreImpl::get_variable(const std::string& name) const
    {
        (void)name;
        throw NotImplemented();
    }

    void StoreImpl::set_variable(const std::string& name, int value)
    {
        (void)name;
        (void)value;
        throw NotImplemented();
    }

    std::map<std::string, int> StoreImpl::get_inventory() const
    {
        throw NotImplemented();
    }

} // namespace libzork::store
