#ifndef CODEUTILS
#define CODEUTILS
namespace CHARCODES {
class Utils {
public:
  static int ConvertUTF16LEToUTF16BE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars);
  static int ConvertUTF16BEToUTF16LE(const unsigned short* pSrc, const int nSrcLen, unsigned short* pOut, int maxChars);
  static int USC2ToSJIS(const unsigned short* pSource, unsigned char*  pOut, unsigned int n);
  static int SJISToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n);
  static int USC2ToUTF8(const unsigned short* pSource, unsigned char*  pOut, unsigned int n);
  static int UTF8ToUSC2(const unsigned char*  pSource, unsigned short* pOut, unsigned int n);
  static int SJISToUTF8(const unsigned char*  pSource, unsigned char*  pOut, unsigned int n);
  static int UTF8ToSJIS(const unsigned char*  pSource, unsigned char*  pOut, unsigned int n);
};
} // namespace CHARCODES
#endif //CODEUTILS
