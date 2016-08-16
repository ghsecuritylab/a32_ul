// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef EXTENSIONS_BROWSER_GUEST_VIEW_WEB_VIEW_WEB_VIEW_CONSTANTS_H_
#define EXTENSIONS_BROWSER_GUEST_VIEW_WEB_VIEW_WEB_VIEW_CONSTANTS_H_

namespace webview {

extern const char kAttributeAllowTransparency[];
extern const char kAttributeAutoSize[];
extern const char kAttributeMaxHeight[];
extern const char kAttributeMaxWidth[];
extern const char kAttributeMinHeight[];
extern const char kAttributeMinWidth[];
extern const char kAttributeName[];
extern const char kAttributeSrc[];

extern const char kAPINamespace[];

extern const char kEventClose[];
extern const char kEventConsoleMessage[];
extern const char kEventContentLoad[];
extern const char kEventContextMenu[];
extern const char kEventDialog[];
extern const char kEventExit[];
extern const char kEventFindReply[];
extern const char kEventFrameNameChanged[];
extern const char kEventLoadAbort[];
extern const char kEventLoadCommit[];
extern const char kEventLoadProgress[];
extern const char kEventLoadRedirect[];
extern const char kEventLoadStart[];
extern const char kEventLoadStop[];
extern const char kEventMessage[];
extern const char kEventNewWindow[];
extern const char kEventPermissionRequest[];
extern const char kEventPluginDestroyed[];
extern const char kEventResponsive[];
extern const char kEventSizeChanged[];
extern const char kEventUnresponsive[];
extern const char kEventZoomChange[];

extern const char kWebViewEventPrefix[];

extern const char kContextMenuItems[];
extern const char kDefaultPromptText[];
extern const char kFindSearchText[];
extern const char kFindFinalUpdate[];
extern const char kInitialHeight[];
extern const char kInitialWidth[];
extern const char kLastUnlockedBySelf[];
extern const char kLevel[];
extern const char kLine[];
extern const char kMessage[];
extern const char kMessageText[];
extern const char kMessageType[];
extern const char kName[];
extern const char kNewHeight[];
extern const char kNewURL[];
extern const char kNewWidth[];
extern const char kOldHeight[];
extern const char kOldURL[];
extern const char kPermission[];
extern const char kPermissionTypeDialog[];
extern const char kPermissionTypeDownload[];
extern const char kPermissionTypeFileSystem[];
extern const char kPermissionTypeGeolocation[];
extern const char kPermissionTypeLoadPlugin[];
extern const char kPermissionTypeMedia[];
extern const char kPermissionTypeNewWindow[];
extern const char kPermissionTypePointerLock[];
extern const char kOldWidth[];
extern const char kProcessId[];
extern const char kProgress[];
extern const char kReason[];
extern const char kRequestId[];
extern const char kSourceId[];
extern const char kTargetURL[];
extern const char kWindowID[];
extern const char kWindowOpenDisposition[];
extern const char kOldZoomFactor[];
extern const char kNewZoomFactor[];

extern const char kInternalBaseURLForDataURL[];
extern const char kInternalCurrentEntryIndex[];
extern const char kInternalEntryCount[];
extern const char kInternalProcessId[];

extern const char kFindNumberOfMatches[];
extern const char kFindActiveMatchOrdinal[];
extern const char kFindSelectionRect[];
extern const char kFindRectLeft[];
extern const char kFindRectTop[];
extern const char kFindRectWidth[];
extern const char kFindRectHeight[];
extern const char kFindCanceled[];
extern const char kFindDone[];

extern const char kParameterUserAgentOverride[];

extern const char kMenuItemCommandId[];
extern const char kMenuItemLabel[];
extern const char kPersistPrefix[];
extern const char kStoragePartitionId[];
extern const unsigned int kMaxOutstandingPermissionRequests;
extern const int kInvalidPermissionRequestID;

}  

#endif  