// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_VIRTUAL_DRIVER_WIN_PORT_MONITOR_PORT_MONITOR_H_
#define CLOUD_PRINT_VIRTUAL_DRIVER_WIN_PORT_MONITOR_PORT_MONITOR_H_

#include <windows.h>
#include <string>
#include "base/files/file_util.h"
#include "base/process/process.h"
#include "base/strings/string16.h"

namespace cloud_print {

base::FilePath GetChromeExePath();

base::FilePath GetChromeProfilePath();


BOOL WINAPI Monitor2EnumPorts(HANDLE,
                              wchar_t*,
                              DWORD level,
                              BYTE*  ports,
                              DWORD   ports_size,
                              DWORD* needed_bytes,
                              DWORD* returned);

BOOL WINAPI Monitor2OpenPort(HANDLE monitor_data, wchar_t*, HANDLE* handle);

BOOL WINAPI Monitor2StartDocPort(HANDLE port_handle,
                                 wchar_t* printer_name,
                                 DWORD job_id,
                                 DWORD,
                                 BYTE*);

BOOL WINAPI Monitor2WritePort(HANDLE port,
                              BYTE*  buffer,
                              DWORD   buffer_size,
                              DWORD* bytes_written);

BOOL WINAPI Monitor2ReadPort(HANDLE, BYTE*, DWORD, DWORD* bytes_read);

BOOL WINAPI Monitor2EndDocPort(HANDLE port_handle);

BOOL WINAPI Monitor2ClosePort(HANDLE port_handle);

VOID WINAPI Monitor2Shutdown(HANDLE monitor_handle);

BOOL WINAPI Monitor2XcvOpenPort(HANDLE monitor,
                                const wchar_t*,
                                ACCESS_MASK granted_access,
                                HANDLE* handle);

DWORD WINAPI Monitor2XcvDataPort(HANDLE xcv_handle,
                                 const wchar_t* data_name,
                                 BYTE*,
                                 DWORD,
                                 BYTE* output_data,
                                 DWORD output_data_bytes,
                                 DWORD* output_data_bytes_needed);

BOOL WINAPI Monitor2XcvClosePort(HANDLE handle);


BOOL WINAPI MonitorUiAddPortUi(const wchar_t*,
                               HWND hwnd,
                               const wchar_t* monitor_name,
                               wchar_t**);

BOOL WINAPI MonitorUiConfigureOrDeletePortUI(const wchar_t*,
                                             HWND hwnd,
                                             const wchar_t* port_name);

extern const wchar_t kChromeExePath[];
extern const wchar_t kChromeExePathRegValue[];
extern const wchar_t kChromeProfilePathRegValue[];
extern const bool kIsUnittest;

}   

#endif  
