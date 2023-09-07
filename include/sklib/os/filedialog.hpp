#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>
#include <variant>
#include <optional>
#include <typeinfo>

namespace sklib::os 
{
    struct file_dialog_error: public std::domain_error
    {
        using std::domain_error::domain_error;
    };

    /**
     * @brief Options for calls to native file dialogs
     */
    struct file_dialog_options 
    {
        explicit file_dialog_options(std::uint8_t flags):
            flags(flags)
        {}

        /**
         * @brief Convert a set of flags into a `file_dialog_options` object
         */
        inline file_dialog_options &operator=(std::uint8_t flags)
        {
            *this = file_dialog_options{flags};
        }

        /**
         * @brief Flags that are passed as options to the native file dialog
         */
        enum mask: std::uint8_t
        {
            use_default_path        = 1 << 0, // open with the provided "defaultPath" member as a context
            use_filter              = 1 << 1, // apply a file filter
            show_hidden_files       = 1 << 2, // show hidden files (.zshrc, ProgramData, whatever...)
            create_if_not_exists    = 1 << 3, // create directories if they dont exist, applies to default path as well
            allow_multiple          = 1 << 4, // allow multiple files / directories to be selected
            use_custom_title        = 1 << 5, // custom window title
            // more options...
        };

        std::uint8_t flags = 0; // Bitmask containing `FileDialogOptions::Flags` values
        std::filesystem::path default_path; // Default path option for the file dialog. Ignored if `flags` does not contain `FileDialogOptions::Flags::WithDefaultPath`
        /* TODO make this a vector of strings: 
        file_dialog_options opts(file_dialog_options::mask::use_filter);
        opts.filters = { "*.txt", "*.docx" };
        open_file(opts);
        */
        std::string filter; // Filter option for the file dialog. Ignored if `flags` does not contain `FileDialogOptions::Flags::WithFilters`
        std::string title; // Title option for the file dialog. Ignored if `flags` does not contain `FileDialogOptions::Flags::CustomTitle`

        inline bool is_flag_set(std::uint8_t flag) const { return flags & flag; }

        inline void set_flag(std::uint8_t flag, bool on) 
        {
            if(on) flags |= flag;
            else flags &= ~flag;
        }
    };

    /**
     * @brief Result of a file dialog operation
     */
    class file_dialog_result
    {
    public:

        using result_type = std::variant<
            std::filesystem::path,
            std::vector<std::filesystem::path>>; 

        template<class T>
        [[nodiscard]] const T &unwrap() const
        {
            if(std::holds_alternative<T>(m_result))
                return std::get<T>(m_result);

            std::string reason = "Could not unwrap T where T = ";
            reason.append(typeid(T).name());
            throw file_dialog_error(reason);
        }

        template<class T>
        [[nodiscard]] T &unwrap()
        {
            if(std::holds_alternative<T>(m_result))
                return std::get<T>(m_result);

            std::string reason = "Could not unwrap T where T = ";
            reason.append(typeid(T).name());
            throw file_dialog_error(reason);
        }

        template<class T>
        [[nodiscard]] std::optional<T> try_unwrap() const noexcept
        {
            try
            {
                return get<T>();
            }
            catch(const std::runtime_error&)
            {
                return std::nullopt;
            }
        }

        inline const result_type &get() const noexcept { return m_result; }
        inline result_type &get() noexcept { return m_result; }
    private:
        friend file_dialog_result open_file(const file_dialog_options&);
        friend file_dialog_result save_file(const file_dialog_options&);
        result_type m_result;
    };

    /**
     * @brief Opens the native file dialog for opening files
     * 
     * @param options Options passed to the file dialog which are translated natively 
     * @return `FileDialogResult` if the file dialog has been confirmed, `std::nullopt` if canceled 
     */
    [[nodiscard]] static file_dialog_result open_file(const file_dialog_options &options);
    /**
     * @brief Opens the native file dialog for saving files
     * 
     * @param options Options passed to the file dialog which are translated natively 
     * @return `FileDialogResult` if the file dialog has been confirmed, `std::nullopt` if canceled 
     */
    [[nodiscard]] static file_dialog_result save_file(const file_dialog_options &options);
}