#include <iostream>
#include <libbeaver/beaver.hh>

int main(void)
{
    auto bearer_roll = beaver::make_beaver("Frankulin", "video/video.mp4");

    bearer_roll->display_video();
    std::cout << bearer_roll->get_name() << std::endl;
}
