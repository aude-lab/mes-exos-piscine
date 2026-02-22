#include <libzork/vars/action.hh>

#include "exceptions.hh"
#include "vars/action_impl.hh"

namespace libzork::vars
{

    std::unique_ptr<Action> make_action(store::Store& store,
                                        const std::string& variable,
                                        const std::string& action, int value)
    {
	            return std::make_unique<ActionImpl>(store, variable, action, value);
    }

} // namespace libzork::vars
