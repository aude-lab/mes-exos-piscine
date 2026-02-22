#include <libbeaver/beaver.hh>
#include <string>

namespace beaver
{
    class BeaverImpl : public Beaver
    {
    public:
        // FIXME
        BeaverImpl(const std::string& name, const std::string& video_path);

        ~BeaverImpl() override = default;

        const std::string& get_name() const override;

        void display_video() override;

    private:
        std::string name_;
        std::string video_path_;
    };
} // namespace beaver
