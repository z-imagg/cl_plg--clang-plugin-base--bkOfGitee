
#include "base/StrUtil.h"



bool StrUtil::startsWith(  std::string str,   std::string prefix){
    if(prefix.size()==0||str.size()==0){
        return false;
    }

    if(str.find(prefix)==_INDEX_0){
        return true;
    }

    return false;

}