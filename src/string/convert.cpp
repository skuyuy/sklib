#include <sklib/string/convert.hpp>
#include <codecvt>

namespace sklib::string
{
    std::wstring to_wstring(char c)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.from_bytes(c);
    }

    std::wstring to_wstring(const char *str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.from_bytes(str);
    }

    std::wstring to_wstring(std::string_view str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.from_bytes(str.data());
    }

    std::wstring to_wstring(const std::string &str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.from_bytes(str);
    }

    std::string to_string(wchar_t c)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.to_bytes(c);
    }

    std::string to_string(const wchar_t *str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.to_bytes(str);
    }

    std::string to_string(std::wstring_view str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.to_bytes(str.data());
    }

    std::string to_string(const std::wstring &str)
    {
        using convert_type = std::codecvt_utf8_utf16<wchar_t>;
        auto cvt = std::wstring_convert<convert_type>{};

        return cvt.to_bytes(str);
    }
}