#include "beaver_impl.hh"
// FIXME
#include "libplayer/video_player.hh"

namespace beaver
{
    BeaverImpl::BeaverImpl(const std::string& video_path):video_path_(video_path)
    // FIXME
    {}

    void BeaverImpl::display_video()
    {
        // FIXME
        play_video(video_path_);
    }

} // namespace beaver
