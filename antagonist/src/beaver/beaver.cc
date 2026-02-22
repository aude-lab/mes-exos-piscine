#include <memory>

#include "beaver_impl.hh"

namespace beaver
{
    std::unique_ptr<Beaver> make_beaver(const std::string& name,
                                        const std::string& video_path)
    {
        // FIXME
        return std::make_unique<BeaverImpl>(name, video_path);
    }
} // namespace beaver
