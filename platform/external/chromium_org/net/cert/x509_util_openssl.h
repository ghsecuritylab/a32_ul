// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_UTIL_OPENSSL_H_
#define NET_CERT_X509_UTIL_OPENSSL_H_

#include <openssl/asn1.h>
#include <openssl/x509v3.h>

#include <string>
#include <vector>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace base {
class Time;
}  

namespace net {

namespace x509_util {

bool NET_EXPORT ParsePrincipalKeyAndValue(X509_NAME_ENTRY* entry,
                                          std::string* key,
                                          std::string* value);

bool NET_EXPORT ParsePrincipalKeyAndValueByIndex(X509_NAME* name,
                                                 int index,
                                                 std::string* key,
                                                 std::string* value);

bool NET_EXPORT ParsePrincipalValueByIndex(X509_NAME* name,
                                           int index,
                                           std::string* value);

bool NET_EXPORT ParsePrincipalValueByNID(X509_NAME* name,
                                         int nid,
                                         std::string* value);

bool NET_EXPORT ParseDate(ASN1_TIME* x509_time, base::Time* time);

bool NET_EXPORT GetDER(X509* x509, base::StringPiece* out_der);

} 

} 

#endif  