#pragma once

#include <libzork/store/store.hh>

namespace libzork::store
{

    class StoreImpl : public Store
    {
    public:
        ~StoreImpl() override = default;

        const story::Node* get_active_node() const override;
        void set_active_node(const story::Node* node) override;

        bool has_variable(const std::string& name) const override;
        int get_variable(const std::string& name) const override;
        void set_variable(const std::string& name, int value) override;
        std::map<std::string, int> get_inventory() const override;

    private:
        const story::Node* active_node_ = nullptr;
        std::map<std::string, int> variables_;
    };

} // namespace libzork::store
