#include <sklib/os.hpp>
#include <iostream>

using namespace sklib::os;

int main()
{
    file_dialog_options options;

    options.set_flag(file_dialog_options::use_custom_title, true);
    options.set_flag(file_dialog_options::allow_multiple, true);
    options.title = "My File Dialog";

    auto result = open_file(options);

    return 0;
}
