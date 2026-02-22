#pragma once

#include <libzork/vars/condition.hh>

namespace libzork::store
{
    class Store;
}
namespace libzork::vars
{

    class ConditionImpl : public Condition
    {
    public:
        ConditionImpl(const store::Store& store, const std::string& variable,
                      const std::string& comparison, int value);
        ~ConditionImpl() override = default;

        bool apply() const override;

    private:
        const store::Store& store_;
        std::string variable_;
        std::string comparison_;
        int value_;
    };

} // namespace libzork::vars
