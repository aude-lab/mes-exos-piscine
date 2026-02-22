#include "runner/choice_impl.hh"

#include <libzork/exceptions.hh>

#include "exceptions.hh"
#include "story/node_impl.hh"

namespace libzork::runner
{
    ChoiceRunnerImpl::ChoiceRunnerImpl(std::unique_ptr<story::Story> story,
                                       std::istream& is, std::ostream& os)
        : ChoiceRunner(std::move(story), is, os)
    {}

    void ChoiceRunnerImpl::print_script() const
    {
        const auto* current = this->story_->get_current();
        if (!current)
            throw std::runtime_error("No active node");
        this->os_ << current->get_text() << "\n";
        const auto choices = current->list_choices(true);
        for (std::size_t i = 0; i < choices.size(); ++i)
            this->os_ << (i + 1) << ". " << choices[i] << "\n";
        this->os_ << "\n";
    }

    void ChoiceRunnerImpl::process_input()
    {
        std::string input;
        std::getline(this->is_, input);

        const auto* current = this->story_->get_current();
        const int n =
            static_cast<int>(current->list_choices(true).size());

        int choice_idx = 0;
        try
        {
            choice_idx = std::stoi(input);
        }
        catch (const std::exception&)
        {
            throw RunnerInterrupt("Please input an integer between 1 and "
                                  + std::to_string(n));
        }

        if (choice_idx < 1 || choice_idx > n)
            throw RunnerInterrupt("Please input an integer between 1 and "
                                  + std::to_string(n));

        const auto& node_impl = story::to_impl(*current);
        int visible_count = 0;
        const std::size_t total_raw = current->list_choices(false).size();
        for (std::size_t i = 0; i < total_raw; ++i)
        {
            if (node_impl.check_conditions(i))
            {
                visible_count++;
                if (visible_count == choice_idx)
                {
                    const auto* next = current->get_choice(i, true);
                    if (next)
                    {
                        this->story_->set_current(next);
                        return;
                    }
                }
            }
        }

        throw RunnerInterrupt("Please input an integer between 1 and "
                              + std::to_string(n));
    }

} // namespace libzork::runner
