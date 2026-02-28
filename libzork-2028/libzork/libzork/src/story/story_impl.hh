#ifndef STORY_IMPL_HH
#define STORY_IMPL_HH

#include <libzork/story/story.hh>

namespace libzork::story
{

    class StoryImpl : public Story
    {
    public:
        ~StoryImpl() override = default;

        const std::string& get_title() const override;
        const Node* get_current() const override;
        void set_current(const Node* node) override;
        const store::Store* get_store() const override;
        std::ostream& display(std::ostream& os) const override;
    };

    private:
        std::string title_;
        std::vector<std::unique_ptr<Node>> nodes_;
        std::map<std::string, Node*> nodes_by_name_;
        std::unique_ptr<store::Store> store_;

    const StoryImpl& to_impl(const Story& story);
    StoryImpl& to_impl(Story& story);

} // namespace libzork::story
#endif // !STORY_IMPL_HH
