#pragma once
#include <sklib/vfs/node.hpp>

namespace sklib::vfs
{
    /**
     * @brief Initialize the virtual path mapping
     */
    void init() noexcept(false);
    /**
     * @brief Try to initialize the virtual path mapping
     */
    inline bool try_init() noexcept
    {
        try
        {
            init();
            return true;
        }
        catch(...)
        {
            return false;
        }
    }
    /**
     * @brief Get the path object from an input vpath
     */
    [[nodiscard]] std::filesystem::path get_path(std::string_view vpath) noexcept;
    /**
     * @brief Get the path object from a node
     */
    [[nodiscard]] inline std::filesystem::path get_path(std::string_view vpath, const node& node) noexcept { return node.get_path(vpath); };
    /**
     * @brief Get the node object with the specified name
     */
    [[nodiscard]] std::optional<node> get_node(std::string_view node_name) noexcept;
}
