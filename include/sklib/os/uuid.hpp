#pragma once
#include <memory>
#include <string>

namespace sklib::os
{
    class uuid
    {
    public:
        uuid();
        uuid(const uuid &other);
        uuid &operator=(const uuid &other);
        uuid(uuid&&) noexcept = default;
        uuid &operator=(uuid&&) noexcept = default;
        ~uuid();

        static void from_string(std::string_view string, uuid &uuid) noexcept;
        static void to_string(std::string &string, const uuid &uuid) noexcept;

        bool is_nil() const noexcept;
        operator bool() const noexcept;

        bool operator==(const uuid &other) const noexcept;
        inline bool operator!=(const uuid &other) const noexcept { return !(*this == other); };

        friend struct std::hash<uuid>;
    private:
        struct impl;
        std::unique_ptr<impl> m;
    };
}
