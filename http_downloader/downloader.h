// Author: Weida Lei (2279793184@qq.com)
//

#pragma once

#include <mutex>
#include <string>
#include <vector>

namespace downloader {

// 下载器基类
class Downloader {
public:
  static size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream);

  void MergeFiles(const std::string &output_file, int threads);
  void Download(const std::string &url, const std::string &output_file,
                int threads);

  virtual ~Downloader() {}
  virtual void BeforeDownload(const std::string &, const std::string &, int) {}
  virtual void AfterDownload(const std::string &, const std::string &, int) {}
  // 获取文件大小失败时返回 -1
  virtual long GetFileSize(const std::string &url) = 0;
  virtual void DownloadChunk(const std::string &url,
                             const std::string &output_file, int start, int end,
                             int thread_num) = 0;

private:
  // 用于写入文件的互斥锁
  inline static std::mutex write_mutex_;
};

} // namespace downloader
