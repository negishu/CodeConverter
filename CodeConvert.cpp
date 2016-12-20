#include <fstream>
#include <iomanip>
#include "CodeConvert.h"
CodeConvert::CodeConvert(void)
{
}
CodeConvert::~CodeConvert(void)
{
}
void CodeConvert::Compile(const char* pFileName, const char* pOutFileName)
{
    LoadFileA(pFileName);
    unsigned short __UNICODE[0xFFFF+1] = {0,};
    unsigned char  __CODE[0xFFFF+1][2] = {0,};
    if ( _LineListA.size() > 0) {
        std::vector<std::string> col;
        for (std::vector<std::string>::iterator it = _LineListA.begin(); it != _LineListA.end(); ++it) {
            col.clear();
            std::string line = *it;
            if (line[0] == '#') continue;
            spliterA(line, '\t', &col);
            const std::string &code    = col[0];
            const std::string &unicode = col[1];
            if (code.length() <= 6 && unicode.length() <= 6) {
                int ncode    = ::strtoul(code.c_str(), 0, 16);
                int nunicode = ::strtoul(unicode.c_str(), 0, 16);
                if (ncode <= 0xFFFF && nunicode <= 0xFFFF) {
                    __UNICODE[ncode] = nunicode;
                    if (ncode <= 0xFF) {
                        __CODE[nunicode][0] = ncode;
                        __CODE[nunicode][1] = 0;
                    }
                    else {
                        __CODE[nunicode][0] = (ncode & 0xFF00) >> 8;
                        __CODE[nunicode][1] = (ncode & 0x00FF) ;
                    }
                }
            }
        }
    }

    std::string h_filename = pOutFileName; h_filename += ".h";
    std::string cpp_filename = pOutFileName; cpp_filename += ".cpp";
    {
        std::ofstream ofs(cpp_filename, std::ofstream::out);
        ofs.setf(std::ios::hex, std::ios::basefield);
        ofs.setf(std::ios::right);
        ofs.setf(std::ios::fixed);
        ofs.setf(std::ios::showbase);

        if (ofs.is_open()) {
            ofs << "#include\"" << h_filename << "\"" << std::endl;
            ofs << "namespace CHARCODES {" << std::endl;
            for (int i = 0; i <= 0xFFFF; i++) {
                if ((0 <= i && i <= 0xFF) || (0x8000 <= i && i <= 0x99FF) || (0xE000 <= i && i <= 0xFFFF)) {
                    if (i % 16 == 0) {
                        if (i == 0x0) {
                            ofs << "static const unsigned short __SJISToUCS2_0000_00FF[] = {" << std::endl;
                        }
                        else
                            if (i == 0x8000) {
                                ofs << "static const unsigned short __SJISToUCS2_8000_99FF[] = {" << std::endl;
                            }
                            else
                                if (i == 0xE000) {
                                    ofs << "static const unsigned short __SJISToUCS2_E000_FFFF[] = {" << std::endl;
                                }
                        ofs << "/* " << std::setw(6) << i << " - " << std::setw(6) << (i + 15) << " */ ";
                    }
                    ofs << std::setw(6) << __UNICODE[i] << ",";
                    if (i % 16 == 15) {
                        if (i == 0xFF) {
                            ofs << std::endl;
                            ofs << "};";
                        }
                        else
                            if (i == 0x99FF) {
                                ofs << std::endl;
                                ofs << "};";
                            }
                            else
                                if (i == 0xFFFF) {
                                    ofs << std::endl;
                                    ofs << "};";
                                }
                        ofs << std::endl;
                    }
                }
            }
            for (int i = 0; i <= 0xFFFF; i++) {
                if ((0 <= i && i <= 0x04FF) || (0x2000 <= i && i <= 0x26FF) || (0x3000 <= i && i <= 0x33CF) || (0x4E00 <= i && i <= 0x9FFF) || (0xF800 <= i && i <= 0xFFFF)) {
                    if (i % 16 == 0) {
                        if (i == 0x0) {
                            ofs << "static const char* __UCS2ToSJIS_0000_04FF[] = {" << std::endl;
                        }
                        else
                            if (i == 0x2000) {
                                ofs << "static const char* __UCS2ToSJIS_2000_26FF[] = {" << std::endl;
                            }
                            else
                                if (i == 0x3000) {
                                    ofs << "static const char* __UCS2ToSJIS_3000_33CF[] = {" << std::endl;
                                }
                                else
                                    if (i == 0x4E00) {
                                        ofs << "static const char* __UCS2ToSJIS_4E00_9FFF[] = {" << std::endl;
                                    }
                                    else
                                        if (i == 0xF800) {
                                            ofs << "static const char* __UCS2ToSJIS_F800_FFFF[] = {" << std::endl;
                                        }
                        ofs << "/* " << std::setw(6) << i << " - " << std::setw(6) << (i + 15) << " */ ";
                    }
                    if (__CODE[i][0]) {
                        ofs << "{" << std::setw(4) << (int)__CODE[i][0];
                        if (__CODE[i][1]) {
                            ofs << "," << std::setw(4) << (int)__CODE[i][1];
                        }
                        ofs << ",0x00},";
                    }
                    else {
                        ofs << "0x00,";
                    }
                    if (i % 16 == 15) {
                        if (i == 0x04FF) {
                            ofs << std::endl;
                            ofs << "};";
                        }
                        else
                            if (i == 0x26FF) {
                                ofs << std::endl;
                                ofs << "};";
                            }
                            else
                                if (i == 0x33CF) {
                                    ofs << std::endl;
                                    ofs << "};";
                                }
                                else
                                    if (i == 0x9FFF) {
                                        ofs << std::endl;
                                        ofs << "};";
                                    }
                                    else
                                        if (i == 0xFFFF) {
                                            ofs << std::endl;
                                            ofs << "};";
                                        }
                        ofs << std::endl;
                    }
                }
            }
            ofs << "int Utils::ConvertUTF16LEToUTF16BE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    const unsigned short *begin = pSrc;" << std::endl;
            ofs << "    const unsigned short *end   = pSrc + nSrcLen;" << std::endl;
            ofs << "    int n = 0;" << std::endl;
            ofs << "    for (; n < maxChars && begin + n < end; n++) {" << std::endl;
            ofs << "        pOut[n] = ((*(begin + n) & 0x00ff) << 8 | (*(begin + n) & 0xff00) >> 8);" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[n] = 0;" << std::endl;
            ofs << "    return n;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::ConvertUTF16BEToUTF16LE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    const unsigned short *begin = pSrc;" << std::endl;
            ofs << "    const unsigned short *end   = pSrc + nSrcLen;" << std::endl;
            ofs << "    int n = 0;" << std::endl;
            ofs << "    for (; n < maxChars && begin + n < end; n++) {" << std::endl;
            ofs << "        pOut[n] = ((*(begin + n) & 0x00ff) << 8 | (*(begin + n) & 0xff00) >> 8);" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[n] = 0;" << std::endl;
            ofs << "    return n;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::USC2ToSJIS(const unsigned short* pSource, unsigned char*  pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource + nReadIndex) != 0));nReadIndex++) {" << std::endl;
            ofs << "        unsigned short c = pSource[nReadIndex];" << std::endl;
            ofs << "        const char* p = 0;" << std::endl;
            ofs << "        if (0x0000 <= c && c <= 0x04FF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_0000_04FF[c];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x2000 <= c && c <= 0x26FF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_2000_26FF[c - 0x2000];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x3000 <= c && c <= 0x33CF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_3000_33CF[c - 0x3000];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x4E00 <= c && c <= 0x9FFF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_4E00_9FFF[c - 0x4E00];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0xF800 <= c && c <= 0xFFFF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_F800_FFFF[c - 0xF800];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        if (p) {" << std::endl;
            ofs << "            while (*p) {" << std::endl;
            ofs << "                pOut[index++] = *p; p++;" << std::endl;
            ofs << "            }" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[index] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::SJISToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource+nReadIndex) != 0));) {" << std::endl;
            ofs << "        const unsigned char c = pSource[nReadIndex++];" << std::endl;
            ofs << "        unsigned short C = 0;" << std::endl;
            ofs << "        if ((0x80 <= c && c <= 0x9F)) {" << std::endl;
            ofs << "            C = c << 8 | pSource[nReadIndex++];" << std::endl;
            ofs << "            C = __SJISToUSC2_8000_99FF[(C - 0x8000)];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((0xE0 <= c && c <= 0xFF)) {" << std::endl;
            ofs << "            C = c << 8 | pSource[nReadIndex++];" << std::endl;
            ofs << "            C = __SJISToUSC2_E000_FFFF[(C-0xE000)];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((0x00 <= c && c <= 0xFF)) {" << std::endl;
            ofs << "            C = __SJISToUSC2_00_FF[c];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        if (pOut) pOut[index] = C;" << std::endl;
            ofs << "        index++;" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[index] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::USC2ToUTF8(const unsigned short* pSource, unsigned char*  pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource + nReadIndex) != 0)); nReadIndex++) {" << std::endl;
            ofs << "        if (*pSource <= 0x007f) {" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0x007f));" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (*pSource <= 0x07ff) {" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0x07c0) >> 6) | 0xc0;" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0x003f)) | 0x80;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else {" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0xf000) >> 12) | 0xe0;" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0x0fc0) >> 6) | 0x80;" << std::endl;
            ofs << "            pOut[index++] = ((*pSource & 0x003f)) | 0x80;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[index++] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::UTF8ToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource + nReadIndex) != 0)); ) {" << std::endl;
            ofs << "        if ((*(pSource + nReadIndex) & 0x80) == 0x00) {" << std::endl;
            ofs << "            pOut[index++] = ((*(pSource + nReadIndex) & 0x7f));" << std::endl;
            ofs << "            nReadIndex++;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((*(pSource + nReadIndex) & 0xE0) == 0xc0) {" << std::endl;
            ofs << "            pOut[index++] = (((*(pSource + nReadIndex) & 0x3f) << 6) | (*(pSource + nReadIndex + 1) & 0x003f));" << std::endl;
            ofs << "            nReadIndex += 2;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else {" << std::endl;
            ofs << "            pOut[index++] = (((*(pSource + nReadIndex) & 0x3f) << 12) | ((*(pSource + nReadIndex + 1) & 0x003f) << 6) | (*(pSource + nReadIndex + 2) & 0x003f));" << std::endl;
            ofs << "            nReadIndex += 3;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[index++] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::SJISToUTF8(const unsigned char* pSource, unsigned char* pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource + nReadIndex) != 0)); ) {" << std::endl;
            ofs << "        const unsigned char c = pSource[nReadIndex++];" << std::endl;
            ofs << "        unsigned short C = 0;" << std::endl;
            ofs << "        if ((0x80 <= c && c <= 0x9F)) {" << std::endl;
            ofs << "            C = c << 8 | pSource[nReadIndex++];" << std::endl;
            ofs << "            C = __SJISToUSC2_8000_99FF[(C - 0x8000)];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((0xE0 <= c && c <= 0xFF)) {" << std::endl;
            ofs << "            C = c << 8 | pSource[nReadIndex++];" << std::endl;
            ofs << "            C = __SJISToUSC2_E000_FFFF[(C - 0xE000)];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((0x00 <= c && c <= 0xFF)) {" << std::endl;
            ofs << "            C = __SJISToUSC2_00_FF[c];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        if (C <= 0x007F) {" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0x007F));" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (C <= 0x07ff) {" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0x07c0) >> 6) | 0xc0;" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0x003f)) | 0x80;" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else {" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0xf000) >> 12) | 0xe0;" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0x0fc0) >> 6) | 0x80;" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "            if (pOut) pOut[index] = ((C & 0x003f)) | 0x80;" << std::endl;
            ofs << "            index++;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    if (pOut) pOut[index] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "int Utils::UTF8ToSJIS(const unsigned char* pSource, unsigned char* pOut, unsigned int n)" << std::endl;
            ofs << "{" << std::endl;
            ofs << "    unsigned int index = 0;" << std::endl;
            ofs << "    for (unsigned int nReadIndex = 0; ((index < n) && (*(pSource + nReadIndex) != 0)); ) {" << std::endl;
            ofs << "        unsigned short C = 0;" << std::endl;
            ofs << "        if ((*(pSource + nReadIndex) & 0x80) == 0x00) {" << std::endl;
            ofs << "            C = ((*(pSource + nReadIndex) & 0x7f));" << std::endl;
            ofs << "            nReadIndex++;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if ((*(pSource + nReadIndex) & 0xE0) == 0xC0) {" << std::endl;
            ofs << "            C = (((*(pSource + nReadIndex) & 0x3f) << 6) | (*(pSource + nReadIndex + 1) & 0x003f));" << std::endl;
            ofs << "             nReadIndex += 2;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else {" << std::endl;
            ofs << "            C = (((*(pSource + nReadIndex) & 0x3f) << 12) | ((*(pSource + nReadIndex + 1) & 0x003f) << 6) | (*(pSource + nReadIndex + 2) & 0x003f));" << std::endl;
            ofs << "            nReadIndex += 3;" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        const char* p = 0;" << std::endl;
            ofs << "        if (0x0000 <= C && C <= 0x04FF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_0000_04FF[C];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x2000 <= C && C <= 0x26FF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_2000_26FF[C - 0x2000];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x3000 <= C && C <= 0x33CF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_3000_33CF[C - 0x3000];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0x4E00 <= C && C <= 0x9FFF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_4E00_9FFF[C - 0x4E00];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        else" << std::endl;
            ofs << "        if (0xF800 <= C && C <= 0xFFFF) {" << std::endl;
            ofs << "            p = __USC2ToSJIS_F800_FFFF[C - 0xF800];" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "        if (p) {" << std::endl;
            ofs << "            while (*p) {" << std::endl;
            ofs << "                pOut[index++] = *p; p++;" << std::endl;
            ofs << "            }" << std::endl;
            ofs << "        }" << std::endl;
            ofs << "    }" << std::endl;
            ofs << "    pOut[index++] = 0;" << std::endl;
            ofs << "    return index;" << std::endl;
            ofs << "}" << std::endl;
            ofs << "} // namespace CHARCODES" << std::endl;
        }
        ofs.close();
    }
    {
        std::ofstream ofs(h_filename, std::ofstream::out);
        ofs.setf(std::ios::hex, std::ios::basefield);
        ofs.setf(std::ios::right);
        ofs.setf(std::ios::fixed);
        if (ofs.is_open()) {
            ofs << "#ifndef CODEUTILS" << std::endl;
            ofs << "#define CODEUTILS" << std::endl;
            ofs << "namespace CHARCODES {" << std::endl;
            ofs << "class Utils {" << std::endl;
            ofs << "public:" << std::endl;
            ofs << "  static int ConvertUTF16LEToUTF16BE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars);" << std::endl;
            ofs << "  static int ConvertUTF16BEToUTF16LE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars);" << std::endl;
            ofs << "  static int USC2ToSJIS(const unsigned short* pSource, unsigned char*  pOut, unsigned int n);" << std::endl;
            ofs << "  static int SJISToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n);" << std::endl;
            ofs << "  static int USC2ToUTF8(const unsigned short* pSource, unsigned char*  pOut, unsigned int n);" << std::endl;
            ofs << "  static int UTF8ToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n);" << std::endl;
            ofs << "  static int SJISToUTF8(const unsigned char*  pSource, unsigned char*  pOut, unsigned int n);" << std::endl;
            ofs << "  static int UTF8ToSJIS(const unsigned char*  pSource, unsigned char*  pOut, unsigned int n);" << std::endl;
            ofs << "};" << std::endl;
            ofs << "} // namespace CHARCODES" << std::endl;
            ofs << "#endif //CODEUTILS" << std::endl;
        }
    }
}
