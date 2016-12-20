#ifndef CODE_CONVERT
#include "DataList.h"
class CodeConvert : public DataList
{
public:
    CodeConvert(void);
    ~CodeConvert(void);
    void Compile(const char* pInFileName, const char* pOutFileName);
};
#endif
