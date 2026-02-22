#include "vars/action_impl.hh"

#include "exceptions.hh"

namespace libzork::vars
{
    ActionImpl::ActionImpl(store::Store& store, const std::string& variable,
                           const std::string& operation, int value)
        : store_(store)
        , variable_(variable)
        , operation_(operation)
        , value_(value)
    {}

    void ActionImpl::apply() const
    {
        const int current_value = store_.get_variable(variable_);
        int new_value = current_value;

        if (operation_ == "assign")
        {
            new_value = value_;
        }
        else if (operation_ == "add")
        {
            new_value = current_value + value_;
        }
        else if (operation_ == "sub")
        {
            new_value = current_value - value_;
        }
        else
        {
            throw std::invalid_argument("Invalid operation: " + operation_);
        }

        store_.set_variable(variable_, new_value);
    }

} // namespace libzork::vars
