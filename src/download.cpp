//
// Created by tri on 8/2/24.
//
#include <qt/upload_download.h>
#include <util/system.h>

#include <logging.h>

#include <curl/curl.h>
#include <stdio.h>


size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::ofstream* file = static_cast<std::ofstream*>(userdata);
    size_t written = 0;
    if (file->is_open()) {
        file->write(static_cast<char*>(ptr), size * nmemb);
        written = size * nmemb;
    }
    return written;
}


void GetBanList() {
    CURL* curl;
    CURLcode res;

    const std::string output_file = "banlist.dat";


    std::ofstream file(output_file, std::ios::binary); // Open file for writing
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << output_file << std::endl;
        return 1;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        std::string downUrl = "https:/explorer.keymaker.cc/banlist.dat/" 
        //gArgs.GetArg("-ipfsservice", DEFAULT_IPFS_SERVICE_URL) + "get/" + cid;
        curl_easy_setopt(curl, CURLOPT_URL, downUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
       // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            LogPrintf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);


    }
    curl_global_cleanup();
}


