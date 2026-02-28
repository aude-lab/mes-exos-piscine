#ifndef CHOICE_IMPL_HH
#define CHOICE_IMPL_HH

#include <libzork/runner/choice.hh>

namespace libzork::runner
{

    class ChoiceRunnerImpl : public ChoiceRunner
    {
    public:
        ~ChoiceRunnerImpl() override = default;

        void print_script() const override;
        void process_input() override;
    };

} // namespace libzork::runner
#endif // !CHOICE_IMPL_HH
