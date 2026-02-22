#include <libzork/store/store.hh>
#include <libzork/vars/condition.hh>
#include "vars/condition_impl.hh"
#include "exceptions.hh"

namespace libzork::vars
{

    std::unique_ptr<Condition> make_condition(const store::Store& store,
                                              const std::string& variable,
                                              const std::string& comparison,
                                              int value)
    {
	    return std::make_unique<ConditionImpl>(store, variable, comparison,
                                               value);
    }

} // namespace libzork::vars
