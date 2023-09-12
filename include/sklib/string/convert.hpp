#pragma once
#include <string>

namespace sklib::string
{
    std::wstring to_wstring(char c);
    std::wstring to_wstring(const char *str);
    std::wstring to_wstring(std::string_view str);
    std::wstring to_wstring(const std::string &str);

    std::string to_string(wchar_t c);
    std::string to_string(const wchar_t *str);
    std::string to_string(std::wstring_view str);
    std::string to_string(const std::wstring &str);
}