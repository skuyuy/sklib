#include "sklib/os/filedialog.hpp"
#include <AppKit/AppKit.h>
#include <sklib/os/file_dialog.hpp>
#import <Cocoa/Cocoa.h>

// MACOS implementation

namespace sklib::os
{
    file_dialog_result open_file(const file_dialog_options &options)
    {
        NSOpenPanel *openDlg = [NSOpenPanel openPanel];

        [openDlg setCanChooseFiles:YES];
        [openDlg setCanChooseDirectories:NO];
        [openDlg setAllowsMultipleSelection:multiple];
        NSMutableArray *types = [NSMutableArray new];
        // for (size_t idx = 0; idx < filetypes.size(); ++idx)
        //    [types addObject: [NSString stringWithUTF8String: filetypes[idx].first.c_str()]];
        [types addObject: [NSString stringWithUTF8String: options.filter.c_str()]];


        [openDlg setAllowedFileTypes: types];

        if ([openDlg runModal] == NSModalResponseOK) {
            for (NSURL* url in [openDlg URLs]) {
                result.emplace_back((char*) [[url path] UTF8String]);
            }
        }
    }
}