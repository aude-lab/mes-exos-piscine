#include "story/story_impl.hh"

#include "exceptions.hh"

namespace libzork::story
{

    const std::string& StoryImpl::get_title() const
    {
        return title_;
    }

    const Node* StoryImpl::get_current() const
    {
        return store_->get_active_node();
    }

    void StoryImpl::set_current(const Node* node)
    {
        store_->set_active_node(node);
    }

    const store::Store* StoryImpl::get_store() const
    {
        return store_.get();
    }

    const StoryImpl& to_impl(const Story& story)
    {
        return dynamic_cast<const StoryImpl&>(story);
    }

    StoryImpl& to_impl(Story& story)
    {
        return dynamic_cast<StoryImpl&>(story);
    }

    std::ostream& StoryImpl::display(std::ostream& os) const
    {
        os << "digraph story {\n";
        for (const auto& node : nodes_)
        {
            const auto& impl = to_impl(*node);
            const auto all_choices = node->list_choices(false);
            if (all_choices.empty())
                continue;

            os << "    \"" << node->get_name() << "\" -> {";
            for (std::size_t i = 0; i < all_choices.size(); ++i)
            {
                if (i != 0)
                    os << " ";
                const auto* target = impl.get_choice(i, false);
                if (target)
                    os << "\"" << target->get_name() << "\"";
            }
            os << "};\n";
        }
        os << "}\n";
        return os;
    }

} // namespace libzork::story
