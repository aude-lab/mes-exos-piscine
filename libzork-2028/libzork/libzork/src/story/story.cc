#include <libzork/story/story.hh>

#include "exceptions.hh"
#include "story/story_impl.hh"

namespace libzork::story
{

    std::unique_ptr<Story> make_story(const fs::path& path)
    {
        (void)path;
        throw NotImplemented();
    }

} // namespace libzork::story
