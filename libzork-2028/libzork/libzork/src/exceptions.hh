#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>

namespace libzork
{
    class NotImplemented : public std::runtime_error
    {
    public:
        NotImplemented(const char* file = __builtin_FILE(),
                       int line = __builtin_LINE())
            : std::runtime_error(std::string{ "Unimplemented function: " }
                                 + file + ":" + std::to_string(line))
        {}
    };

} // namespace libzork
#endif // !EXCEPTIONS_HH
