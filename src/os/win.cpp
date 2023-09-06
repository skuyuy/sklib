#include <sklib/os/filesystem.hpp>
#include <sklib/os/fileoperation.hpp>
#include <sklib/string.hpp>

#include <codecvt>

// WINDOWS implementation
#include <Windows.h>

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

}
