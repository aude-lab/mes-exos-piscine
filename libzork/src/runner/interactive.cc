#include <libzork/exceptions.hh>
#include <libzork/runner/interactive.hh>

#include "exceptions.hh"

namespace libzork::runner
{

    InteractiveRunner::InteractiveRunner(std::unique_ptr<story::Story> story,
                                         std::istream& is, std::ostream& os)
        : Runner(std::move(story))
        , is_(is)
        , os_(os)
    {}

    void InteractiveRunner::print_script() const
    {
         const auto* current = story_->get_current();
        if (!current)
            return;
        os_ << current->get_text() << "\n";
    }

    void InteractiveRunner::run()
    {
        try
        {
            const auto* current = story_->get_current();
            while (current && !current->list_choices().empty())
            {
                print_script();
                bool valid = false;
                while (!valid)
                {
                    os_ << "> ";
                    try
                    {
                        process_input();
                        valid = true;
                    }
                    catch (const RunnerInterrupt& e)
                    {
                        os_ << e.what() << "\n";
                    }
                }
                current = story_->get_current();
            }
            print_script();
        }
        catch (const RunnerQuit&)
        {
        }
    }

} // namespace libzork::runner
