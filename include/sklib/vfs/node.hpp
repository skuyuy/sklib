#pragma once
#include <string>
#include <filesystem>
#include <optional>

namespace sklib::vfs
{
    /**
     * @brief Represents a virtual path mapping
     */
    class node
    {
    private:
        /**
         * @brief The actual path of the node
         */
        std::filesystem::path mapped_path_;
    public:
        /**
         * @brief Construct an empty node object
         */
        node() = default;
        /**
         * @brief Construct a new node object
         */
        node(const std::filesystem::path &mapped_path);
        /**
         * @brief Destroy the node object
         */
        ~node() = default;
        /**
         * @brief Get the path resulting from appending the input string
         */
        [[nodiscard]] std::filesystem::path get_path(std::string_view path) const noexcept;
    };
}
