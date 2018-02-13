#include "extiostream.h"
#include "iobuf.hpp"

#ifdef _MSC_VER
#define EXTIOS_GLOBAL_DEFINE extern "C" __declspec(dllexport)
#else // _MSC_VER
#define EXTIOS_GLOBAL_DEFINE
#endif // _MSC_VER

static extios::basic_inputbuf<char> u8inputbuf;
static extios::basic_inputbuf<wchar_t> winputbuf;
static extios::basic_inputbuf<char16_t> u16inputbuf;
static extios::basic_inputbuf<char32_t> u32inputbuf;
EXTIOS_GLOBAL_DEFINE extios::ext_basic_istream<char> u8cin(&u8inputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_istream<wchar_t> wcin(&winputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_istream<char16_t> u16cin(&u16inputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_istream<char32_t> u32cin(&u32inputbuf);

static extios::basic_outputbuf<char> u8outputbuf;
static extios::basic_outputbuf<wchar_t> woutputbuf;
static extios::basic_outputbuf<char16_t> u16outputbuf;
static extios::basic_outputbuf<char32_t> u32outputbuf;
EXTIOS_GLOBAL_DEFINE extios::ext_basic_ostream<char> u8cout(&u8outputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_ostream<wchar_t> wcout(&woutputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_ostream<char16_t> u16cout(&u16outputbuf);
EXTIOS_GLOBAL_DEFINE extios::ext_basic_ostream<char32_t> u32cout(&u32outputbuf);
