// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_INFO_MESSAGE_FILTER_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_INFO_MESSAGE_FILTER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/sequenced_task_runner_helpers.h"
#include "chrome/browser/plugins/plugin_prefs.h"
#include "components/content_settings/core/common/content_settings.h"
#include "content/public/browser/browser_message_filter.h"

struct ChromeViewHostMsg_GetPluginInfo_Output;
struct ChromeViewHostMsg_GetPluginInfo_Status;
class GURL;
class HostContentSettingsMap;
class PluginFinder;
class PluginMetadata;
class Profile;

namespace content {
class ResourceContext;
struct WebPluginInfo;
}

class PluginInfoMessageFilter : public content::BrowserMessageFilter {
 public:
  struct GetPluginInfo_Params;

  
  class Context {
   public:
    Context(int render_process_id, Profile* profile);

    ~Context();

    void DecidePluginStatus(
        const GetPluginInfo_Params& params,
        const content::WebPluginInfo& plugin,
        const PluginMetadata* plugin_metadata,
        ChromeViewHostMsg_GetPluginInfo_Status* status) const;
    bool FindEnabledPlugin(int render_frame_id,
                           const GURL& url,
                           const GURL& top_origin_url,
                           const std::string& mime_type,
                           ChromeViewHostMsg_GetPluginInfo_Status* status,
                           content::WebPluginInfo* plugin,
                           std::string* actual_mime_type,
                           scoped_ptr<PluginMetadata>* plugin_metadata) const;
    void GetPluginContentSetting(const content::WebPluginInfo& plugin,
                                 const GURL& policy_url,
                                 const GURL& plugin_url,
                                 const std::string& resource,
                                 ContentSetting* setting,
                                 bool* is_default,
                                 bool* is_managed) const;
    void MaybeGrantAccess(const ChromeViewHostMsg_GetPluginInfo_Status& status,
                          const base::FilePath& path) const;
    bool IsPluginEnabled(const content::WebPluginInfo& plugin) const;

   private:
    int render_process_id_;
    content::ResourceContext* resource_context_;
    const HostContentSettingsMap* host_content_settings_map_;
    scoped_refptr<PluginPrefs> plugin_prefs_;

    BooleanPrefMember allow_outdated_plugins_;
    BooleanPrefMember always_authorize_plugins_;
  };

  PluginInfoMessageFilter(int render_process_id, Profile* profile);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnDestruct() const OVERRIDE;

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<PluginInfoMessageFilter>;

  virtual ~PluginInfoMessageFilter();

  void OnGetPluginInfo(int render_frame_id,
                       const GURL& url,
                       const GURL& top_origin_url,
                       const std::string& mime_type,
                       IPC::Message* reply_msg);

  
  
  void PluginsLoaded(const GetPluginInfo_Params& params,
                     IPC::Message* reply_msg,
                     const std::vector<content::WebPluginInfo>& plugins);

#if defined(ENABLE_PEPPER_CDMS)
  
  
  
  
  
  
  
  void OnIsInternalPluginAvailableForMimeType(
      const std::string& mime_type,
      bool* is_available,
      std::vector<base::string16>* additional_param_names,
      std::vector<base::string16>* additional_param_values);
#endif

  Context context_;

  base::WeakPtrFactory<PluginInfoMessageFilter> weak_ptr_factory_;
  scoped_refptr<base::SingleThreadTaskRunner> main_thread_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(PluginInfoMessageFilter);
};

#endif  
