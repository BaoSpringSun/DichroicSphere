/**
 *
*/
#pragma once

#include "json.h"

class MyJson
{
public:
    MyJson();
    ~MyJson();


private:
    /* data */
    Json::Value     mRoot;
    Json::Reader    mReader;
};





