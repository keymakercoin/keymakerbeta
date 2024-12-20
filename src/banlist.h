//
// Created by tri on 8/2/24.
//

//#ifndef KEYMAKER_BANLIST_H
//#define KEYMAKER_BANLIST_H
#include <string>
#include <iostream>
#include <vector>

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata);


void GetBanList();
//#endif //KEYMAKER_BANLIST_H
