#include "beaver_impl.hh"

namespace beaver
{
    std::unique_ptr<Beaver> make_beaver(const std::string& video_path)
    {
        return std::make_unique<BeaverImpl>(video_path);
    }

} // namespace beaver
