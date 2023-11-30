// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
#include "downloader.h"
#include <fstream>
#include <iostream>
#include <thread>

namespace downloader {

size_t Downloader::WriteData(void *ptr, size_t size, size_t nmemb,
                             void *stream) {
  std::ofstream *out_stream = static_cast<std::ofstream *>(stream);
  std::lock_guard<std::mutex> lock(write_mutex_);
  out_stream->write(static_cast<char *>(ptr), size * nmemb);
  return size * nmemb;
}

void Downloader::MergeFiles(const std::string &output_file, int threads) {
  std::ofstream output(output_file, std::ios::binary);
  if (!output) {
    std::cerr << "Cannot open file for writing: " << output_file << std::endl;
    return;
  }

  for (int i = 0; i < threads; ++i) {
    std::string part_file_name = output_file + ".part" + std::to_string(i);
    std::ifstream part_file(part_file_name, std::ios::binary);
    if (!part_file) {
      std::cerr << "Cannot open part file for reading: " << part_file_name
                << std::endl;
      return;
    }

    output << part_file.rdbuf();

    part_file.close();
    // 删除部分文件
    remove(part_file_name.c_str());
  }
}

void Downloader::Download(const std::string &url,
                          const std::string &output_file, int threads) {
  // 前处理
  BeforeDownload(url, output_file, threads);

  // 创建一个临时文件输出流
  std::ofstream output(output_file, std::ios::binary);
  if (!output) {
    std::cerr << "Cannot open file for writing: " << output_file << std::endl;
    return;
  }

  // 获取文件大小
  long file_size = GetFileSize(url);
  if (file_size == -1) {
    std::cerr << "Cannot get file size, exit!" << std::endl;
    return;
  } else {
    std::cout << "File size is:" << file_size << std::endl;
  }

  // 计算分片大小
  long batch_size = file_size / threads;
  int remainder = file_size % threads;
  long start = 0;
  long end = 0;

  // 分割文件并创建线程
  std::vector<std::thread> thread_pool;
  for (int i = 0; i < threads; ++i) {
    start = end == 0 ? 0 : end + 1;
    if (i < remainder) {
      end = start + batch_size + 1;
    } else {
      end = start + batch_size;
    }
    std::cout << "Part" << i << ", start is:" << start << ", end is:" << end
              << std::endl;
    thread_pool.emplace_back(&Downloader::DownloadChunk, this, url, output_file,
                             start, end, i);
  }

  // 等待所有线程完成
  std::cout << "Downloading ......" << std::endl;
  for (auto &t : thread_pool) {
    if (t.joinable()) {
      t.join();
    }
  }

  // 合并文件
  std::cout << "All part download finished, start merge" << std::endl;
  MergeFiles(output_file, threads);
  std::cout << "All part has been mergered" << std::endl;

  // 后处理
  AfterDownload(url, output_file, threads);
}

} // namespace downloader
