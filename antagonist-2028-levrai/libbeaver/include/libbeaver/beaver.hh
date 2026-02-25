#pragma once

#include <memory>

namespace beaver
{
    class Beaver
    {
    public:
        virtual ~Beaver() = default;

        virtual void display_video() = 0;
    };

    std::unique_ptr<Beaver> make_beaver(const std::string& video_path);

} // namespace beaver
