#ifndef CONDITION_IMPL_HH
#define CONDITION_IMPL_HH

#include <libzork/vars/condition.hh>

namespace libzork::vars
{

    class ConditionImpl : public Condition
    {
    public:
        ~ConditionImpl() override = default;

        bool apply() const override;
    };

} // namespace libzork::vars
#endif // !CONDITION_IMPL_HH
