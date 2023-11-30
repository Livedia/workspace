// Author: Weida Lei (2279793184@qq.com)
//

#pragma once

#include "downloader.h"
#include <mutex>

namespace downloader {

// HTTP 下载器
class HTTPDownloader : public Downloader {
public:
  HTTPDownloader();
  virtual ~HTTPDownloader();

  virtual long GetFileSize(const std::string &url) override;
  virtual void DownloadChunk(const std::string &url,
                             const std::string &output_file, int start, int end,
                             int thread_num) override;
  virtual void BeforeDownload(const std::string &url,
                              const std::string &output_file,
                              int threads) override;
  virtual void AfterDownload(const std::string &url,
                             const std::string &output_file,
                             int threads) override;

private:
  // 用于下载线程的互斥锁
  inline static std::mutex download_mutex_;
};

} // namespace downloader
