#include <sklib/vfs.hpp>

#include <fstream>
#include <unordered_map>
#include <sstream>

#include <nlohmann/json.hpp>
#include <generated/config.hpp>

#ifndef SK_VFS_VPATH_SEPERATOR
    #define SK_VFS_VPATH_SEPERATOR "://"
#endif

struct _vfs_ctx
{
#ifdef SK_VFS_CONFIG_PATH
    const std::filesystem::path config_path = std::filesystem::path(SK_VFS_CONFIG_PATH) / "vfsconfig.json";
#else
    const std::filesystem::path config_path = std::filesystem::current_path() / "vfsconfig.json";
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

    std::stringstream ss;
    ss << ifs.rdbuf();

    const auto config = nlohmann::json::parse(ss.str());
    auto version = config.at("version").get<size_t>();

    if(version != sklib::VERSION_MAJOR) throw std::runtime_error("Major version mismatch in config!");

    for(auto it = config.begin(); it != config.end(); ++it)
    {
        if(it.key() == "version") continue;
        else if(!it->is_string()) continue;

        if(g_vfs_context.node_map.find(it.key()) != g_vfs_context.node_map.end()) continue;

        g_vfs_context.node_map[it.key()] = node{it->get<std::string>()};
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
