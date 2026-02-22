#include <memory>

namespace beaver
{
    class Beaver
    {
    public:
        virtual ~Beaver() = default;

        virtual const std::string& get_name() const = 0;

        virtual void display_video() = 0;
    };

    std::unique_ptr<Beaver> make_beaver(const std::string& name,
                                        const std::string& video_path);

} // namespace beaver
