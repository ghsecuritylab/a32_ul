// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_TEST_UTILS_H_
#define CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_TEST_UTILS_H_

#include "base/files/file_util.h"
#include "base/files/scoped_temp_dir.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "chrome/browser/extensions/api/image_writer_private/image_writer_utility_client.h"
#include "chrome/browser/extensions/api/image_writer_private/operation_manager.h"
#include "content/public/test/test_browser_thread_bundle.h"
#include "content/public/test/test_utils.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(OS_CHROMEOS)
#include "chromeos/disks/disk_mount_manager.h"
#include "chromeos/disks/mock_disk_mount_manager.h"
#endif

namespace extensions {
namespace image_writer {

const char kDummyExtensionId[] = "DummyExtension";

const int kTestFileSize = 32 * 1024;
const int kImagePattern = 0x55555555; 
const int kDevicePattern = 0xAAAAAAAA; 

class MockOperationManager : public OperationManager {
 public:
  MockOperationManager();
  explicit MockOperationManager(content::BrowserContext* context);
  virtual ~MockOperationManager();

  MOCK_METHOD3(OnProgress, void(const ExtensionId& extension_id,
                                image_writer_api::Stage stage,
                                int progress));
  
  MOCK_METHOD1(OnComplete, void(const std::string& extension_id));

  
  MOCK_METHOD4(OnError, void(const ExtensionId& extension_id,
                             image_writer_api::Stage stage,
                             int progress,
                             const std::string& error_message));
};

#if defined(OS_CHROMEOS)
class FakeDiskMountManager : public chromeos::disks::MockDiskMountManager {
 public:
  FakeDiskMountManager();
  virtual ~FakeDiskMountManager();

  virtual void UnmountDeviceRecursively(
      const std::string& device_path,
      const UnmountDeviceRecursivelyCallbackType& callback) OVERRIDE;

 private:
  DiskMap disks_;
};
#endif

class FakeImageWriterClient : public ImageWriterUtilityClient {
 public:
  FakeImageWriterClient();

  virtual void Write(const ProgressCallback& progress_callback,
                     const SuccessCallback& success_callback,
                     const ErrorCallback& error_callback,
                     const base::FilePath& source,
                     const base::FilePath& target) OVERRIDE;

  virtual void Verify(const ProgressCallback& progress_callback,
                      const SuccessCallback& success_callback,
                      const ErrorCallback& error_callback,
                      const base::FilePath& source,
                      const base::FilePath& target) OVERRIDE;

  virtual void Cancel(const CancelCallback& cancel_callback) OVERRIDE;

  virtual void Shutdown() OVERRIDE;

  
  void SetWriteCallback(const base::Closure& write_callback);
  
  void SetVerifyCallback(const base::Closure& verify_callback);

  
  void Progress(int64 progress);
  
  void Success();
  
  void Error(const std::string& message);
  
  void Cancel();

 private:
  virtual ~FakeImageWriterClient();

  ProgressCallback progress_callback_;
  SuccessCallback success_callback_;
  ErrorCallback error_callback_;
  CancelCallback cancel_callback_;

  base::Closure write_callback_;
  base::Closure verify_callback_;
};

class ImageWriterTestUtils {
 public:
  ImageWriterTestUtils();
  virtual ~ImageWriterTestUtils();

  // Verifies that the data in image_path was written to the file at
  
  
  bool ImageWrittenToDevice();

  
  
  bool FillFile(const base::FilePath& file,
                const int pattern,
                const int length);

  
  
  
  virtual void SetUp();
  
  
  
  
  virtual void SetUp(bool is_browser_test);

  virtual void TearDown();

  const base::FilePath& GetTempDir();
  const base::FilePath& GetImagePath();
  const base::FilePath& GetDevicePath();

#if !defined(OS_CHROMEOS)
  FakeImageWriterClient* GetUtilityClient();
#endif

 protected:
  base::ScopedTempDir temp_dir_;
  base::FilePath test_image_path_;
  base::FilePath test_device_path_;

#if !defined(OS_CHROMEOS)
  scoped_refptr<FakeImageWriterClient> client_;
#endif
};

class ImageWriterUnitTestBase : public testing::Test {
 protected:
  ImageWriterUnitTestBase();
  virtual ~ImageWriterUnitTestBase();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  ImageWriterTestUtils test_utils_;

 private:
  content::TestBrowserThreadBundle thread_bundle_;
};

}  
}  

#endif  