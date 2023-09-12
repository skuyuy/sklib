#include <sklib/vfs/node.hpp>

sklib::vfs::node::node(const std::filesystem::path &mapped_path):
    mapped_path_(mapped_path)
{
}

std::filesystem::path sklib::vfs::node::get_path(std::string_view path) const noexcept
{
    return mapped_path_ / path;
}
