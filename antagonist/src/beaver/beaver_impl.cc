#include "beaver_impl.hh"

#include "video_player.hh"

namespace beaver
{
    // FIXME
    BeaverImpl::BeaverImpl(const std::string& name,
                           const std::string& video_path)
        : name_{ name }
        , video_path_{ video_path }
    {}

    const std::string& BeaverImpl::get_name() const
    {
        // FIXME
        return name_;
    }

    void BeaverImpl::display_video()
    {
        // FIXME
        std::string video_path = video_path_;
        play_video(video_path);
    }

} // namespace beaver
