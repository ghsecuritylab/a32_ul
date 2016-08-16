// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_SOCKET_UTILS_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_SOCKET_UTILS_H_

#include "content/public/common/socket_permission_request.h"
#include "ppapi/c/pp_stdint.h"

struct PP_NetAddress_Private;

namespace net {
class X509Certificate;
}

namespace ppapi {
class PPB_X509Certificate_Fields;
}

namespace content {

namespace pepper_socket_utils {

SocketPermissionRequest CreateSocketPermissionRequest(
    SocketPermissionRequest::OperationType type,
    const PP_NetAddress_Private& net_addr);

bool CanUseSocketAPIs(bool external_plugin,
                      bool private_api,
                      const SocketPermissionRequest* params,
                      int render_process_id,
                      int render_frame_id);

bool GetCertificateFields(const net::X509Certificate& cert,
                          ppapi::PPB_X509Certificate_Fields* fields);

bool GetCertificateFields(const char* der,
                          uint32_t length,
                          ppapi::PPB_X509Certificate_Fields* fields);

}  

}  

#endif  