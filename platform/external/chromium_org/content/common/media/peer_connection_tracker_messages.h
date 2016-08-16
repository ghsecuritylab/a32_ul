// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/values.h"
#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START PeerConnectionTrackerMsgStart

IPC_STRUCT_BEGIN(PeerConnectionInfo)
  IPC_STRUCT_MEMBER(int, lid)
  IPC_STRUCT_MEMBER(std::string, rtc_configuration)
  IPC_STRUCT_MEMBER(std::string, constraints)
  IPC_STRUCT_MEMBER(std::string, url)
IPC_STRUCT_END()

IPC_MESSAGE_CONTROL1(PeerConnectionTrackerHost_AddPeerConnection,
                     PeerConnectionInfo )
IPC_MESSAGE_CONTROL1(PeerConnectionTrackerHost_RemovePeerConnection,
                     int )
IPC_MESSAGE_CONTROL3(PeerConnectionTrackerHost_UpdatePeerConnection,
                     int ,
                     std::string ,
                     std::string )
IPC_MESSAGE_CONTROL2(PeerConnectionTrackerHost_AddStats,
                     int ,
                     base::ListValue )
IPC_MESSAGE_CONTROL5(PeerConnectionTrackerHost_GetUserMedia,
                     std::string ,
                     bool ,
                     bool ,
                     
                     
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL0(PeerConnectionTracker_GetAllStats)
IPC_MESSAGE_CONTROL0(PeerConnectionTracker_OnSuspend)