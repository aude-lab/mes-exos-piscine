#include "runner/html_impl.hh"

#include <filesystem>
#include <fstream>
#include <set>

#include "exceptions.hh"
#include "story/node_impl.hh"

namespace fs = std::filesystem;

namespace libzork::runner
{

    HTMLRunnerImpl::HTMLRunnerImpl(std::unique_ptr<story::Story> story,
                                   const fs::path& output_dir)
        : HTMLRunner(std::move(story))
        , output_dir_(output_dir)
    {}

    void HTMLRunnerImpl::run()
    {
        fs::create_directories(output_dir_);

        std::set<const story::Node*> visited;
        std::vector<const story::Node*> queue;

        const auto* start = this->story_->get_current();
        if (!start)
            return;

        queue.push_back(start);
        visited.insert(start);

        while (!queue.empty())
        {
            const auto* node = queue.front();
            queue.erase(queue.begin());

            fs::path filepath =
                output_dir_ / (node->get_name() + ".html");
            std::ofstream ofs(filepath);

            ofs << "<html>\n<body>\n";
            ofs << "<p>" << node->get_text() << "</p>\n";

            const auto choices = node->list_choices(false);
            if (!choices.empty())
            {
                ofs << "<ol>\n";
                const auto& node_impl = story::to_impl(*node);
                const std::size_t total = choices.size();
                for (std::size_t i = 0; i < total; ++i)
                {
                    const auto* target = node_impl.get_choice(i, false);
                    if (!target)
                        continue;
                    fs::path target_path =
                        output_dir_ / (target->get_name() + ".html");
                    ofs << "<li><a href=\""
                        << fs::absolute(target_path).string() << "\">"
                        << choices[i] << "</a></li>\n";

                    if (!visited.count(target))
                    {
                        visited.insert(target);
                        queue.push_back(target);
                    }
                }
                ofs << "</ol>\n";
            }

            ofs << "</body>\n</html>\n";
        }
    }

} // namespace libzork::runner
