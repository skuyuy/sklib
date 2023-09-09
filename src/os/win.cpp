#include <sklib/os/filesystem.hpp>
#include <sklib/os/filedialog.hpp>
#include <sklib/os/uuid.hpp>
#include <sklib/string.hpp>

#include <codecvt>

// WINDOWS implementation
#include <Windows.h>
#pragma comment(lib, "rpcrt4.lib")

namespace sklib::os
{

#ifdef UNICODE
    OPENFILENAMEW _create_ofn(const file_dialog_options &options)
    {
        using mask = file_dialog_options::mask;

        OPENFILENAMEW ofn;
        WCHAR file[260] = { 0 };
        WCHAR currentDir[256] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
        ofn.lStructSize = sizeof(OPENFILENAMEW);
        ofn.lpstrFile = file;
        ofn.nMaxFile = sizeof(file);
        ofn.Flags = OFN_FILEMUSTEXIST;

        if(options.is_flag_set(mask::use_default_path)) ofn.lpstrInitialDir = string::to_wstring(options.default_path.string()).c_str();
        if(options.is_flag_set(mask::use_filter))
        {
            ofn.lpstrFilter = string::to_wstring(options.filter).c_str();
            ofn.nFilterIndex = 1;
        }

        if(options.is_flag_set(mask::show_hidden_files)) ofn.Flags |= OFN_FORCESHOWHIDDEN;
        if(options.is_flag_set(mask::create_if_not_exists)) ofn.Flags |= OFN_CREATEPROMPT;
        if(options.is_flag_set(mask::allow_multiple)) ofn.Flags |= OFN_ALLOWMULTISELECT;
        if(options.is_flag_set(mask::use_custom_title)) ofn.lpstrTitle = string::to_wstring(options.title).c_str();
        return ofn;
    }
#else
    OPENFILENAMEA _create_ofn(const file_dialog_options &options)
    {
        using mask = file_dialog_options::mask;

        OPENFILENAMEA ofn;
        CHAR file[260] = { 0 };
        CHAR currentDir[256] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.lpstrFile = file;
        ofn.nMaxFile = sizeof(file);
        ofn.Flags = OFN_FILEMUSTEXIST;

        if(options.is_flag_set(mask::use_default_path)) ofn.lpstrInitialDir = options.default_path.string().c_str();
        if(options.is_flag_set(mask::use_filter)) ofn.lpstrFilter = options.filter.c_str();
        {
            ofn.lpstrFilter = options.filter.c_str();
            ofn.nFilterIndex = 1;
        }

        if(options.is_flag_set(mask::show_hidden_files)) ofn.Flags |= OFN_FORCESHOWHIDDEN;
        if(options.is_flag_set(mask::create_if_not_exists)) ofn.Flags |= OFN_CREATEPROMPT;
        if(options.is_flag_set(mask::allow_multiple)) ofn.Flags |= OFN_ALLOWMULTISELECT;
        if(options.is_flag_set(mask::use_custom_title)) ofn.lpstrTitle = options.title.c_str();

        return ofn;
    }
#endif

file_dialog_result open_file(const file_dialog_options &options)
{
    using mask = file_dialog_options::mask;

    file_dialog_result result;
    auto ofn = _create_ofn(options);
#ifndef UNICODE
    if(GetOpenFileNameA(&ofn) != 0)
    {
        if(options.is_flag_set(mask::allow_multiple))
        {
            std::vector<std::filesystem::path> files;

            char* buf = ofn.lpstrFile;
            std::filesystem::path dir{buf};

            buf += dir.string().length() + 1; // advance to the next file *behind* the directory and one more char (to skip \0)

            while(*buf)
            {
                std::filesystem::path file{buf};

                files.push_back(dir / file);
                buf += file.string().length() + 1;
            }

            result.m_result = files;
        }
        else result.m_result = std::filesystem::path{ofn.lpstrFile};
    }
#else
    if(GetOpenFileNameW(&ofn) != 0)
    {
        if(options.is_flag_set(mask::allow_multiple))
        {
            std::vector<std::filesystem::path> files;

            wchar_t* buf = ofn.lpstrFile;
            std::wstring dir_str{buf};
            std::filesystem::path dir{string::to_string(dir_str)};

            buf += dir_str.length() + 1; // advance to the next file *behind* the directory and one more char (to skip \0)

            while(*buf)
            {
                std::wstring file_str{buf};
                std::filesystem::path file{string::to_string(file_str)};

                files.push_back(dir / file);
                buf += file_str.length() + 1;
            }

            result.m_result = files;
        }
        else result.m_result = std::filesystem::path{string::to_string(ofn.lpstrFile)};
    }
#endif
    return result;
}

file_dialog_result save_file(const file_dialog_options &options)
{
    file_dialog_result result;
    auto ofn = _create_ofn(options);

#ifdef UNICODE
    if(GetSaveFileNameW(&ofn) != 0) result.m_result = std::filesystem::path{string::to_string(ofn.lpstrFile)};
#else
    if(GetSaveFileNameA(&ofn) != 0) result.m_result = std::filesystem::path{ofn.lpstrFile};
#endif

    return result;
}

struct uuid::impl
{
    UUID uuid;
};

uuid::uuid():
    m{std::make_unique<impl>()}
{
    UuidCreateNil(&m->uuid);
}

uuid::uuid(const uuid &other):
    m{std::make_unique<impl>(*other.m)}
{}

uuid &uuid::operator=(const uuid &other)
{
    m = std::make_unique<impl>(*other.m);
    return *this;
}

uuid::~uuid() = default;

void uuid::from_string(std::string_view string, uuid &uuid) noexcept
{
#ifdef UNICODE
    auto wstr = string::to_wstring(string.data());
    auto result = UuidFromStringW((RPC_WSTR) wstr.data(), &uuid.m->uuid);
#else
    auto result = UuidFromStringA((RPC_CSTR) string.data(), &uuid.m->uuid);
#endif
    _ASSERT_EXPR(result == RPC_S_OK, L"Failed to convert string to UUID");
}

void uuid::to_string(std::string &string, const uuid &uuid) noexcept
{
#ifdef UNICODE
    using convert_type = std::codecvt_utf8_utf16<wchar_t>;
    auto sc = std::wstring_convert<convert_type>{};

    // we can assume this returns RPC_S_OK because else the system is just out of memory
    std::wstring wstr;
    UuidToStringW(&uuid.m->uuid, (RPC_WSTR*) wstr.data());
    string = string::to_string(wstr);
#else
    UuidToStringA(&uuid.m->uuid, (RPC_CSTR*) string.data());
#endif
}

bool uuid::is_nil() const noexcept
{
    RPC_STATUS status;
    return UuidIsNil(&m->uuid, &status);
}

uuid::operator bool() const noexcept
{
    return !is_nil();
}

bool uuid::operator==(const uuid &other) const noexcept
{
    RPC_STATUS status;
    return UuidEqual(&m->uuid, &other.m->uuid, &status);
}

}
