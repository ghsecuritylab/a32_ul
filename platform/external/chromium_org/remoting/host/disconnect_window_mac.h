// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include <string>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"

@interface DisconnectWindowController : NSWindowController {
 @private
  base::Closure disconnect_callback_;
  base::string16 username_;
  IBOutlet NSTextField* connectedToField_;
  IBOutlet NSButton* disconnectButton_;
}

- (id)initWithCallback:(const base::Closure&)disconnect_callback
              username:(const std::string&)username;
- (IBAction)stopSharing:(id)sender;
@end

@interface DisconnectWindow : NSWindow
@end

@interface DisconnectView : NSView
@end
