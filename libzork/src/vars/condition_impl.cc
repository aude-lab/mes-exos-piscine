#include "vars/condition_impl.hh"

#include <algorithm>
#include <map>

#include "exceptions.hh"

namespace libzork::vars
{
    ConditionImpl::ConditionImpl(const store::Store& store,
                                 const std::string& variable,
                                 const std::string& comparison, int value)
        : store_(store)
        , variable_(variable)
        , comparison_(comparison)
        , value_(value)
    {}

    bool ConditionImpl::apply() const
    {
        const int variable_value = store_.get_variable(variable_);

        if (comparison_ == "equal")
        {
            return variable_value == value_;
        }
        else if (comparison_ == "not_equal")
        {
            return variable_value != value_;
        }
        else if (comparison_ == "greater")
        {
            return variable_value > value_;
        }
        else if (comparison_ == "lower")
        {
            return variable_value < value_;
        }
        else if (comparison_ == "greater_equal")
        {
            return variable_value >= value_;
        }
        else if (comparison_ == "lower_equal")
        {
            return variable_value <= value_;
        }
        else
        {
            throw std::invalid_argument("Invalid comparison operator: "
                                        + comparison_);
        }
    }

} // namespace libzork::vars
