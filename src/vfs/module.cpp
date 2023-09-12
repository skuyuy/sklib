#include <sklib/vfs.hpp>

#include <fstream>
#include <unordered_map>
#include <sstream>

#include <generated/config.hpp>

#ifndef SK_VFS_VPATH_SEPERATOR
    #define SK_VFS_VPATH_SEPERATOR "://"
#endif

struct _vfs_ctx
{
#ifdef SK_VFS_CONFIG_PATH
    const std::filesystem::path config_path = std::filesystem::path(SK_VFS_CONFIG_PATH) / "vfsconfig";
#else
    const std::filesystem::path config_path = std::filesystem::current_path() / "vfsconfig";
#endif
    std::unordered_map<std::string, sklib::vfs::node> node_map;
};

namespace
{
    static _vfs_ctx g_vfs_context;
}

void sklib::vfs::init()
{
    std::ifstream ifs(g_vfs_context.config_path, std::ifstream::in);
    if(!ifs.is_open()) throw std::runtime_error("Could not open vfs config file!");

    std::string line;
    while(std::getline(ifs, line))
    {
        auto sep = line.find("=");
        if(sep == std::string::npos) throw std::runtime_error("Could not parse vfs config file");;

        auto key = line.substr(0, sep);
        auto val = line.substr(sep + 1);

        if(key == "version")
        {
            if(std::stoi(val) != sklib::VERSION_MAJOR) throw std::runtime_error("Major version mismatch in config!");
            else continue;
        }
    	else
        {
            if(g_vfs_context.node_map.find(key) != g_vfs_context.node_map.end()) continue;
            g_vfs_context.node_map[key] = node{val};
        }
    }
}

std::filesystem::path sklib::vfs::get_path(std::string_view vpath) noexcept
{
    static constexpr std::string_view vpath_seperator = SK_VFS_VPATH_SEPERATOR;

    auto sep_pos = vpath.find(vpath_seperator);
    if(sep_pos == std::string::npos) return {};

    auto node_name = vpath.substr(0, sep_pos); // everything before seperator is node
    auto sub_vpath = vpath.substr(sep_pos + vpath_seperator.length()); // everything after seperator is path

    auto node_it = g_vfs_context.node_map.find(std::string(node_name));
    if(node_it == g_vfs_context.node_map.end()) return {};

    return node_it->second.get_path(sub_vpath);
}

std::optional<sklib::vfs::node> sklib::vfs::get_node(std::string_view node_name) noexcept
{
    auto node_it = g_vfs_context.node_map.find(node_name.data());
    if(node_it == g_vfs_context.node_map.end()) return std::nullopt;

    return node_it->second;
}
