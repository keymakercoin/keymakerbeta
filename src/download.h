//
// Created by tri on 8/2/24.
//

#ifndef KEYMAKER_UPLOADDOWNLOAD_H
#define KEYMAKER_UPLOADDOWNLOAD_H
#include <string>
#include <iostream>
#include <vector>

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata);

//void downloadFile(const std::string& cid, const std::string& response_data);
void downloadBanList();
#endif //KEYMAKER_UPLOADDOWNLOAD_H
