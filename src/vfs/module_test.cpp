#include <sklib/vfs.hpp>
#include <iostream>

int main()
{
    if(!sklib::vfs::try_init())
    {
        std::cerr << "could not init vfs\n";
        return false;
    }

    auto path = sklib::vfs::get_path("res://test.txt");

    auto node = sklib::vfs::get_node("res");
    if(!node)
    {
        std::cout << "could not get node 'res'\n";
    }
    else
    {
        auto path2 = sklib::vfs::get_path("test2.txt", *node);
        auto path3 = node->get_path("test3.txt");
    }
}
