#include "CodeConvert.h"

int main(int argc, char* argv[])
{
    CodeConvert aCodeConvert;
    aCodeConvert.Compile("data\\sjis-0213-2004-std.txt","charcodes");
    return 0;
}