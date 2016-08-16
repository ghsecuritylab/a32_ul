// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_HISTORY_MENU_COCOA_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_HISTORY_MENU_COCOA_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/history_menu_bridge.h"

@interface HistoryMenuCocoaController : NSObject {
 @private
  HistoryMenuBridge* bridge_;  
}

- (id)initWithBridge:(HistoryMenuBridge*)bridge;

- (IBAction)openHistoryMenuItem:(id)sender;

@end  

@interface HistoryMenuCocoaController (ExposedForUnitTests)
- (void)openURLForItem:(const HistoryMenuBridge::HistoryItem*)node;
@end  

#endif  