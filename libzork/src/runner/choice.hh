#pragma once
#include <libzork/runner/interactive.hh>

namespace libzork::runner
{
    class ChoiceRunner : public InteractiveRunner
    {
    public:
        using InteractiveRunner::InteractiveRunner;

        void print_script() const override;
        void process_input() override;
    };
} // namespace libzork::runner