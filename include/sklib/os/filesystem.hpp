#pragma once
#include <filesystem>

namespace sklib::os
{
#ifdef APPLICATION_ID
    /**
     * @brief Resolves the cache path of the defined application
     * 
     * Windows  | %AppData%/Local/$APPLICATION_ID/cache
     * macOS    | $HOME/Library/Caches/$APPLICATION_ID
     * Linux    | $XDG_CACHE_HOME/$APPLICATION_ID
     *          | $HOME/.cache/$APPLICATION_ID
     */
    std::filesystem::path project_cache_dir();
    /**
     * @brief Resolves the config path of the defined application
     * 
     * Windows  | %AppData%/Roaming/$APPLICATION_ID/config
     * macOS    | $HOME/Library/Application Support/$APPLICATION_ID
     * Linux    | $XDG_CONFIG_HOME/$APPLICATION_ID
     *          | $HOME/.config/$APPLICATION_ID
     */
    std::filesystem::path project_config_dir();
    /**
     * @brief Resolves the data path of the defined application
     * 
     * Windows  | %AppData%/Roaming/$APPLICATION_ID/data
     * macOS    | $HOME/Library/Application Support/$APPLICATION_ID
     * Linux    | $XDG_DATA_HOME/$APPLICATION_ID
     *          | $HOME/.local/share/$APPLICATION_ID
     */
    std::filesystem::path project_data_dir(bool local);
#endif
    /**
     * @brief Resolves the cache path
     * 
     * Windows  | %AppData%/Local/$APPLICATION_ID/cache
     * macOS    | $HOME/Library/Caches/$APPLICATION_ID
     * Linux    | $XDG_CACHE_HOME/$APPLICATION_ID
     *          | $HOME/.cache/$APPLICATION_ID
     */
    std::filesystem::path cache_dir();
    /**
     * @brief Resolves the config path
     * 
     * Windows  | %AppData%/Roaming/$APPLICATION_ID/config
     * macOS    | $HOME/Library/Application Support/$APPLICATION_ID
     * Linux    | $XDG_CONFIG_HOME/$APPLICATION_ID
     *          | $HOME/.config/$APPLICATION_ID
     */
    std::filesystem::path config_dir();
    /**
     * @brief Resolves the data path
     * 
     * Windows  | %AppData%/Roaming/$APPLICATION_ID/data
     * macOS    | $HOME/Library/Application Support/$APPLICATION_ID
     * Linux    | $XDG_DATA_HOME/$APPLICATION_ID
     *          | $HOME/.local/share/$APPLICATION_ID
     * 
     * @param Local Flag to resolve the local data instead of the common.
     * Only Applies to windows
     */
    std::filesystem::path data_dir(bool local);

    enum class user_dir_type
    {
        home,
        desktop,
        documents,
        download,
        // ...
    };

    /**
     * @brief Resolves the temp path of the defined application
     * 
     * Windows  | %AppData%/Local/temp
     * macOS    | $HOME/Library/Application Support/$APPLICATION_ID
     * Linux    | $XDG_DATA_HOME/$APPLICATION_ID
     *          | $HOME/.local/share/$APPLICATION_ID
     */
    std::filesystem::path user_dir(user_dir_type type);
}   