
#ifndef MyAssert_H
#define MyAssert_H

#include <iostream>
#include <stdexcept>
#define MyAssert(ok_cond,err_msg)     \
if(!(ok_cond)) {                       \
try  {                                \
throw std::runtime_error(err_msg);         \
}    catch(const std::exception& e) { \
std::cout<<__FILE__<<":"<< __LINE__<<":"<<e.what()<<std::endl;       \
throw e;                             \
}                                    \
}

/*void __MyAssert__example(){
    //由于MyAssert整体是个if{...}，因此结尾不需要分号
    MyAssert(1/0,"[AssertErr]NotFit:typeClassEqRecord||typeClassEqElaborated")
}*/
#endif //MyAssert_H
