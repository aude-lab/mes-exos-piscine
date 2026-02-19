#include "stdin_to_file.hh"

int main()
{
    auto word_count = stdin_to_file("file.out");
    std::cout << "File has " << word_count << " words\n";
    return 0;
}
