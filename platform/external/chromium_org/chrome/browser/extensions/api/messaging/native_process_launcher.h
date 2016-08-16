// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_PROCESS_LAUNCHER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_PROCESS_LAUNCHER_H_

#include "base/callback_forward.h"
#include "base/files/file.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "ui/gfx/native_widget_types.h"

class GURL;

namespace base {
class CommandLine;
class FilePath;
}

namespace extensions {

class NativeProcessLauncher {
 public:
  enum LaunchResult {
    RESULT_SUCCESS,
    RESULT_INVALID_NAME,
    RESULT_NOT_FOUND,
    RESULT_FORBIDDEN,
    RESULT_FAILED_TO_START,
  };

  
  
  
  typedef base::Callback<void(LaunchResult result,
                              base::ProcessHandle process_handle,
                              base::File read_file,
                              base::File write_file)> LaunchedCallback;

  static scoped_ptr<NativeProcessLauncher> CreateDefault(
      bool allow_user_level_hosts,
      gfx::NativeView native_view);

  NativeProcessLauncher() {}
  virtual ~NativeProcessLauncher() {}

  
  
  
  
  
  
  virtual void Launch(const GURL& origin,
                      const std::string& native_host_name,
                      LaunchedCallback callback) const = 0;

 protected:
  
  

  
  
  
  static base::FilePath FindManifest(const std::string& native_host_name,
                                     bool allow_user_level_hosts,
                                     std::string* error_message);

  
  static bool LaunchNativeProcess(const base::CommandLine& command_line,
                                  base::ProcessHandle* process_handle,
                                  base::File* read_file,
                                  base::File* write_file);

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeProcessLauncher);
};

}  

#endif  