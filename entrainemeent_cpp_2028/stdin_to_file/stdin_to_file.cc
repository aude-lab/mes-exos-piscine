#include <iostream>
#include <fstream>

long int stdin_to_file(const std::string& filename)
{
    std::string word;
    std::ofstream file_out(filename);
    long int count = 0;
    while (std::cin >> word)
    {
        count++;
        file_out << word << '\n';
    }
 
    return count;

}
