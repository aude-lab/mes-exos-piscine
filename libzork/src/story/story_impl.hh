#pragma once

#include <libzork/story/story.hh>
#include <vector>

namespace libzork::story
{

    class StoryImpl : public Story
    {
    public:
        explicit StoryImpl(const fs::path& path);
        ~StoryImpl() override = default;

        const std::string& get_title() const override;
        const Node* get_current() const override;
        void set_current(const Node* node) override;
        const store::Store* get_store() const override;
        std::ostream& display(std::ostream& os) const override;

        const Node* find_node(const std::string& name) const;

    private:
        std::string title_;
        std::unique_ptr<store::Store> store_;
        std::vector<std::unique_ptr<Node>> nodes_;
        std::map<std::string, Node*> nodes_by_name_;
    };

    const StoryImpl& to_impl(const Story& story);
    StoryImpl& to_impl(Story& story);

} // namespace libzork::story
