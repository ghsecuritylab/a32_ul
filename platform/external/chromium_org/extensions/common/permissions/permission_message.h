// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_MESSAGE_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_MESSAGE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace extensions {

class PermissionMessage {
 public:
  
  
  enum ID {
    kUnknown,
    kNone,
    kBookmarks,
    kGeolocation,
    kBrowsingHistory,
    kTabs,
    kManagement,
    kDebugger,
    kDesktopCapture,
    kHid,
    kHosts1,
    kHosts2,
    kHosts3,
    kHosts4OrMore,
    kHostsAll,
    kFullAccess,
    kClipboard,
    kTtsEngine,
    kContentSettings,
    kPrivacy,
    kSupervisedUser,
    kInput,
    kAudioCapture,
    kVideoCapture,
    kDownloads,
    kDeleted_FileSystemWrite,
    kMediaGalleriesAllGalleriesRead,
    kSerial,
    kSocketAnyHost,
    kSocketDomainHosts,
    kSocketSpecificHosts,
    kBluetooth,
    kUsb,
    kSystemIndicator,
    kUsbDevice,
    kMediaGalleriesAllGalleriesCopyTo,
    kSystemInfoDisplay,
    kNativeMessaging,
    kSyncFileSystem,
    kAudio,
    kFavicon,
    kMusicManagerPrivate,
    kWebConnectable,
    kActivityLogPrivate,
    kBluetoothDevices,
    kDownloadsOpen,
    kNetworkingPrivate,
    kDeclarativeWebRequest,
    kFileSystemDirectory,
    kFileSystemWriteDirectory,
    kSignedInDevices,
    kWallpaper,
    kNetworkState,
    kHomepage,
    kSearchProvider,
    kStartupPages,
    kMediaGalleriesAllGalleriesDelete,
    kScreenlockPrivate,
    kOverrideBookmarksUI,
    kAutomation,
    kAccessibilityFeaturesModify,
    kAccessibilityFeaturesRead,
    kBluetoothPrivate,
    kIdentityEmail,
    kExperienceSamplingPrivate,
    kCopresence,
    kTopSites,
    kU2fDevices,
    kEnumBoundary,
  };
  COMPILE_ASSERT(PermissionMessage::kNone > PermissionMessage::kUnknown,
                 kNone_not_greater_than_kUnknown);

  
  PermissionMessage(ID id, const base::string16& message);
  PermissionMessage(ID id,
                    const base::string16& message,
                    const base::string16& details);
  ~PermissionMessage();

  
  
  ID id() const { return id_; }

  
  
  const base::string16& message() const { return message_; }

  
  
  
  const base::string16& details() const { return details_; }

  
  bool operator<(const PermissionMessage& that) const {
    return id_ < that.id_;
  }
  
  bool operator>(const PermissionMessage& that) const {
    return id_ > that.id_;
  }

 private:
  ID id_;
  base::string16 message_;
  base::string16 details_;
};

typedef std::vector<PermissionMessage> PermissionMessages;

}  

#endif  
