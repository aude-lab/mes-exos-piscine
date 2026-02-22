#include "story/story_impl.hh"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "store/store_impl.hh"
#include "story/node_impl.hh"

namespace libzork::story
{

	StoryImpl::StoryImpl(const fs::path& path)
        : store_(std::make_unique<store::StoreImpl>())
    {
        if (!fs::exists(path))
            throw std::invalid_argument("Story file does not exist");

        YAML::Node config = YAML::LoadFile(path.string());

        title_ = config["title"].as<std::string>();

        fs::path scripts_path = path.parent_path();
        if (config["scripts-path"])
            scripts_path /= config["scripts-path"].as<std::string>();

        if (config["variables"])
        {
            for (const auto& var : config["variables"])
            {
                std::string name = var["name"].as<std::string>();
                int value = var["value"].as<int>();
                store_->set_variable(name, value);
            }
        }

        for (const auto& node_entry : config["story"])
        {
            std::string name = node_entry["name"].as<std::string>();
            fs::path script_path =
                scripts_path / node_entry["script"].as<std::string>();
            auto node = make_node(name, script_path);
            nodes_by_name_[name] = node.get();
            nodes_.push_back(std::move(node));
        }

        for (std::size_t i = 0; i < nodes_.size(); ++i)
        {
            const auto& node_entry = config["story"][i];
            Node& node = *nodes_[i];

            if (!node_entry["choices"])
                continue;

            for (const auto& choice_entry : node_entry["choices"])
            {
                std::string target_name =
                    choice_entry["target"].as<std::string>();
                std::string text = choice_entry["text"].as<std::string>();

                auto it = nodes_by_name_.find(target_name);
                if (it == nodes_by_name_.end())
                    throw std::invalid_argument("Invalid target node: "
                                                + target_name);

                std::vector<std::unique_ptr<vars::Condition>> conditions;
                if (choice_entry["conditions"])
                {
                    for (const auto& cond : choice_entry["conditions"])
                    {
                        conditions.push_back(vars::make_condition(
                            *store_,
                            cond["name"].as<std::string>(),
                            cond["comparison"].as<std::string>(),
                            cond["value"].as<int>()));
                    }
                }

                std::vector<std::unique_ptr<vars::Action>> actions;
                if (choice_entry["actions"])
                {
                    for (const auto& act : choice_entry["actions"])
                    {
                        actions.push_back(vars::make_action(
                            *store_,
                            act["name"].as<std::string>(),
                            act["operation"].as<std::string>(),
                            act["value"].as<int>()));
                    }
                }

                to_impl(node).add_choice(it->second, text,
                                         std::move(conditions),
                                         std::move(actions));
            }
        }

        if (!nodes_.empty())
            store_->set_active_node(nodes_.front().get());
    }

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

    const Node* StoryImpl::find_node(const std::string& name) const
    {
        auto it = nodes_by_name_.find(name);
        return (it != nodes_by_name_.end()) ? it->second : nullptr;
    }

    std::ostream& StoryImpl::display(std::ostream& os) const
    {
        os << "digraph story {\n";
        for (const auto& node : nodes_)
        {
            const NodeImpl& node_impl = to_impl(*node);
            const auto all_choices = node->list_choices(false);
            if (!all_choices.empty())
            {
                os << "    \"" << node->get_name() << "\" -> { ";
                for (std::size_t i = 0; i < all_choices.size(); ++i)
                {
                    if (i != 0)
                        os << " ";
                    const auto* target = node_impl.get_choice(i, false);
                    if (target)
                        os << "\"" << target->get_name() << "\"";
                }
                os << " };\n";
            }
        }
        os << "}\n";
        return os;
    }

    const StoryImpl& to_impl(const Story& story)
    {
        return dynamic_cast<const StoryImpl&>(story);
    }

    StoryImpl& to_impl(Story& story)
    {
        return dynamic_cast<StoryImpl&>(story);
    }


} // namespace libzork::story
