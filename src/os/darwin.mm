#include <AppKit/AppKit.h>
#include <filesystem>

#include <sklib/os/filedialog.hpp>

#import <Cocoa/Cocoa.h>

// MACOS implementation

namespace sklib::os
{
    file_dialog_result open_file(const file_dialog_options &options)
    {
        using mask = file_dialog_options::mask;

        NSOpenPanel *openDlg = [NSOpenPanel openPanel];

        // add assertions for flags that are not supported but set regardless 

        [openDlg setCanChooseFiles:YES];
        [openDlg setCanChooseDirectories:NO]; // TODO add option for choosing directories
        [openDlg setAllowsMultipleSelection:options.is_flag_set(mask::allow_multiple)];
        NSMutableArray *types = [NSMutableArray new];
        // for (size_t idx = 0; idx < filetypes.size(); ++idx)
        //    [types addObject: [NSString stringWithUTF8String: filetypes[idx].first.c_str()]];
        [types addObject: [NSString stringWithUTF8String: options.filter.c_str()]];


        [openDlg setAllowedFileTypes: types];
        std::vector<std::filesystem::path> paths;

        if ([openDlg runModal] == NSModalResponseOK) {
            for (NSURL* url in [openDlg URLs]) {
                paths.push_back(std::filesystem::path{(char*) [[url path] UTF8String]});
            }
        }

        file_dialog_result result;
        result.m_result = paths;
        return result;
    }
}