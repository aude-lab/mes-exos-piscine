#pragma once

#include <libzork/vars/action.hh>
#include <string>

namespace libzork::vars
{

    class ActionImpl : public Action
    {
    public:
        ActionImpl(store::Store& store, const std::string& variable,
                   const std::string& operation, int value);
        ~ActionImpl() override = default;

        void apply() const override;

    private:
        store::Store& store_;
        std::string variable_;
        std::string operation_;
        int value_;
    };

} // namespace libzork::vars
