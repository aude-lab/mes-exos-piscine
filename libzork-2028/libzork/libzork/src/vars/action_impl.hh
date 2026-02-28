#ifndef ACTION_IMPL_HH
#define ACTION_IMPL_HH

#include <libzork/vars/action.hh>

namespace libzork::vars
{

    class ActionImpl : public Action
    {
    public:
        ~ActionImpl() override = default;

        void apply() const override;
    };

} // namespace libzork::vars
#endif // !ACTION_IMPL_HH
