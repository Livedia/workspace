// Author: Weida Lei (2279793184@qq.com)
//

#include "http_downloader.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

namespace downloader {

HTTPDownloader::HTTPDownloader() {
  // 初始化 curl
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

HTTPDownloader::~HTTPDownloader() {
  // 清理 curl
  curl_global_cleanup();
}

long HTTPDownloader::GetFileSize(const std::string &url) {
  long fileSize = -1;
  CURL *curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // 不需要 body，只需要 HTTP header
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    // 不需要返回 header 信息
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    // 如果文件被重定向，则自动跟随重定向
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
      res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T,
                              &fileSize);
      if (res != CURLE_OK) {
        std::cerr << "Error when getting file size: " << curl_easy_strerror(res)
                  << std::endl;
      }
    } else {
      std::cerr << "Error when performing HEAD request: "
                << curl_easy_strerror(res) << std::endl;
    }
    curl_easy_cleanup(curl);
  }
  return fileSize;
}

void HTTPDownloader::DownloadChunk(const std::string &url,
                                   const std::string &output_file, int start,
                                   int end, int thread_num) {
  // 使用 libcurl 下载指定范围的文件部分
  // 初始化 CURL
  CURL *curl = curl_easy_init();
  if (curl == nullptr) {
    std::lock_guard<std::mutex> lock(download_mutex_);
    std::cerr << "Failed to initialize libcurl" << std::endl;
    return;
  }

  // 设置 CURL 选项
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
  std::ofstream output(output_file + ".part" + std::to_string(thread_num),
                       std::ios::binary);
  if (!output) {
    std::lock_guard<std::mutex> lock(download_mutex_);
    std::cerr << "Unable to create file: " << output_file + ".part"
              << std::endl;
    return;
  }
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

  // 设置下载范围
  std::string range = std::to_string(start) + "-" + std::to_string(end);
  curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());

  // 执行 CURL 请求
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::lock_guard<std::mutex> lock(download_mutex_);
    std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
  }

  // 清理
  output.close();
  curl_easy_cleanup(curl);
}

void HTTPDownloader::BeforeDownload(const std::string &url,
                                    const std::string &output_file,
                                    int threads) {
  std::cout << "Download file url:" << url << std::endl;
  std::cout << "Download output file:" << output_file << std::endl;
  std::cout << "Download use " << threads << " threads" << std::endl;
}

void HTTPDownloader::AfterDownload(const std::string &,
                                   const std::string &output_file, int) {
  std::cout << "Download finished, output file is:" << output_file << std::endl;
}

} // namespace downloader
