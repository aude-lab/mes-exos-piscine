#include <iostream>
#include <libbeaver/beaver.hh>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_video>\n";
        return 1;
    }

    std::string video_path = argv[1];
    auto bearer_roll = beaver::make_beaver(video_path);
    bearer_roll->display_video();
}
