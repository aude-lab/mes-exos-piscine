#ifndef HTML_IMPL_HH
#define HTML_IMPL_HH

#include <libzork/runner/html.hh>

namespace libzork::runner
{

    class HTMLRunnerImpl : public HTMLRunner
    {
    public:
        ~HTMLRunnerImpl() override = default;

        void run() override;
    };

} // namespace libzork::runner
#endif // !HTML_IMPL_HH
