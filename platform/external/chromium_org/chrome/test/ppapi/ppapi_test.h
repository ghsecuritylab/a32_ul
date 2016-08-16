// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_PPAPI_PPAPI_TEST_H_
#define CHROME_TEST_PPAPI_PPAPI_TEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/test/javascript_test_observer.h"
#include "net/test/spawned_test_server/spawned_test_server.h"

namespace content {
class RenderViewHost;
}

class PPAPITestMessageHandler : public content::TestMessageHandler {
 public:
  PPAPITestMessageHandler();

  virtual MessageResponse HandleMessage(const std::string& json) OVERRIDE;
  virtual void Reset() OVERRIDE;

  const std::string& message() const {
    return message_;
  }

 private:
  std::string message_;

  DISALLOW_COPY_AND_ASSIGN(PPAPITestMessageHandler);
};

class PPAPITestBase : public InProcessBrowserTest {
 public:
  PPAPITestBase();

  
  virtual void SetUp() OVERRIDE;
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;

  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) = 0;

  
  GURL GetTestFileUrl(const std::string& test_case);
  virtual void RunTest(const std::string& test_case);
  virtual void RunTestViaHTTP(const std::string& test_case);
  virtual void RunTestWithSSLServer(const std::string& test_case);
  virtual void RunTestWithWebSocketServer(const std::string& test_case);
  virtual void RunTestIfAudioOutputAvailable(const std::string& test_case);
  virtual void RunTestViaHTTPIfAudioOutputAvailable(
      const std::string& test_case);

 protected:
  class InfoBarObserver : public content::NotificationObserver {
   public:
    explicit InfoBarObserver(PPAPITestBase* test_base);
    ~InfoBarObserver();

    void ExpectInfoBarAndAccept(bool should_accept);

   private:
    
    virtual void Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) OVERRIDE;

    void VerifyInfoBarState();

    content::NotificationRegistrar registrar_;
    PPAPITestBase* test_base_;
    bool expecting_infobar_;
    bool should_accept_;
  };

  
  
  void RunTestURL(const GURL& test_url);
  
  
  GURL GetTestURL(const net::SpawnedTestServer& http_server,
                  const std::string& test_case,
                  const std::string& extra_params);
};

class PPAPITest : public PPAPITestBase {
 public:
  PPAPITest();

  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;

  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
 protected:
  bool in_process_;  
};

class PPAPIPrivateTest : public PPAPITest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class OutOfProcessPPAPITest : public PPAPITest {
 public:
  OutOfProcessPPAPITest();

  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class OutOfProcessPPAPIPrivateTest : public OutOfProcessPPAPITest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class PPAPINaClTest : public PPAPITestBase {
 public:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;
  
  virtual void RunTest(const std::string& test_case) OVERRIDE;
  virtual void RunTestViaHTTP(const std::string& test_case) OVERRIDE;
  virtual void RunTestWithSSLServer(const std::string& test_case) OVERRIDE;
  virtual void RunTestWithWebSocketServer(
      const std::string& test_case) OVERRIDE;
  virtual void RunTestIfAudioOutputAvailable(
      const std::string& test_case) OVERRIDE;
  virtual void RunTestViaHTTPIfAudioOutputAvailable(
      const std::string& test_case) OVERRIDE;
};

class PPAPINaClNewlibTest : public PPAPINaClTest {
 public:
  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
};

class PPAPIPrivateNaClNewlibTest : public PPAPINaClNewlibTest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class PPAPINaClGLibcTest : public PPAPINaClTest {
 public:
  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
};

class PPAPIPrivateNaClGLibcTest : public PPAPINaClGLibcTest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class PPAPINaClPNaClTest : public PPAPINaClTest {
 public:
  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
};

class PPAPIPrivateNaClPNaClTest : public PPAPINaClPNaClTest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class PPAPINaClPNaClNonSfiTest : public PPAPINaClTest {
 public:
  virtual void SetUpCommandLine(base::CommandLine* command_line);

  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
};

class PPAPIPrivateNaClPNaClNonSfiTest : public PPAPINaClPNaClNonSfiTest {
 protected:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;
};

class PPAPINaClTestDisallowedSockets : public PPAPITestBase {
 public:
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;

  virtual std::string BuildQuery(const std::string& base,
                                 const std::string& test_case) OVERRIDE;
};

class PPAPIBrokerInfoBarTest : public OutOfProcessPPAPITest {
 public:
  
  virtual void SetUpOnMainThread() OVERRIDE;
};

#endif  