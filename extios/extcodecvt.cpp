#include "extcodecvt.h"
#include <stdexcept> // std::invalid_argument std::length_error
#include <type_traits> // std::is_same

#define throw_if_cvtor_null(cvtor)\
if (cvtor == nullptr)\
{\
	throw std::invalid_argument("编码转换对象不能是空");\
}

#define throw_if_string_too_long(length)\
if ((length) > static_cast<std::size_t>(std::numeric_limits<int>::max()))\
{\
	throw std::length_error("需要转换编码的字符串过长");\
}

#ifdef _MSC_VER

#include <numeric> // std::numeric_limits
#include <Windows.h> // MultiByteToWideChar WideCharToMultiByte

#undef max

struct extios::codecvtor_base::member_data
{
};


void * extios::codecvtor_base::handle(void) const noexcept
{
	return nullptr;
}


// UTF-32转UTF-16或者UTF-32转宽字符, 输出对应字符的std::basic_string或者std::vector
// OnputContainer 容器的返回类型
// s 需要转换的字符串
// n 需要转换的字符串的字符数
template <typename OnputContainer>
static OnputContainer to_utf16(const char32_t *s, std::size_t n)
{
	using cahr_type = typename OnputContainer::value_type;
	constexpr auto iswchar = std::is_same<cahr_type, wchar_t>::value;
	constexpr auto ischar16 = std::is_same<cahr_type, char16_t>::value;
	static_assert(iswchar || ischar16, "The output container must be a wide-character or UTF-16 encoding.");

	OnputContainer container;
	container.reserve(n * 2);

	for (std::size_t i = 0; i < n; ++i)
	{
		auto c = s[i];
		if (c < 0x10000)
		{
			container.push_back(static_cast<cahr_type>(c));
			continue;
		}
		auto t = c - 0x10000;
		auto h = (((t << 12) >> 22) + 0xD800);
		auto l = (((t << 22) >> 22) + 0xDC00);
		container.push_back(static_cast<cahr_type>(h));
		container.push_back(static_cast<cahr_type>(l));
	}

	container.shrink_to_fit();
	return container;
}


// UTF-8/窄字符转成std::vector<char16_t>或者std::vector<wchar_t>
// charT char16_t 或者 wchar_t
// s 需要转换的字符串
// n 需要转换的字符串的字节数, 不能超过int的最大值
// CodePage 输入字符串的字符编码
template <typename charT>
static std::vector<charT> to_utf16(const char *s, int n, UINT CodePage)
{
	if (n <= 0)
	{
		return std::vector<charT>();
	}

	constexpr auto iswchar = std::is_same<charT, wchar_t>::value;
	constexpr auto ischar16 = std::is_same<charT, char16_t>::value;
	static_assert(iswchar || ischar16, "The charT must be wchar_t or char16_t.");

	auto length = ::MultiByteToWideChar(CodePage, 0, s, n, nullptr, 0);
	if (length == 0)
	{
		throw std::invalid_argument("需要转换编码的字符串不是有效的字符串");
	}

	std::vector<charT> buffer(length);
	wchar_t *pbuffer = reinterpret_cast<wchar_t *>(buffer.data());
	::MultiByteToWideChar(CodePage, 0, s, n, pbuffer, static_cast<int>(buffer.size()));
	return buffer;
}


// UTF-16/宽字符转成std::u32string/std::vector<char32_t>
// OnputContainer 容器的返回类型
// InputCharType wchar_t 或 char16_t
// s 需要转换的字符串
// n 需要转换的字符串的字符数
template <typename OnputContainer, typename InputCharType>
static OnputContainer to_utf32(const InputCharType *s, std::size_t n)
{
	constexpr auto iswchar = std::is_same<InputCharType, wchar_t>::value;
	constexpr auto ischar16 = std::is_same<InputCharType, char16_t>::value;
	static_assert(iswchar || ischar16, "The input string must be a wide-character or UTF-16 encoding.");
	static_assert(std::is_same<OnputContainer::value_type, char32_t>::value, "The output container must be an UTF-32 encoding.");

	OnputContainer container;
	container.reserve(n);

	auto iter = s;
	auto last = s + n;
	while (iter != last)
	{
		const char16_t prev = *iter;
		++iter;
		if (static_cast<std::size_t>(prev - 0xd800) < 2048)
		{
			if (iter != last)
			{
				const char16_t curr = *iter;
				auto is_high_surrogate = (prev & 0xfffffc00) == 0xd800;
				auto is_low_surrogate = (curr & 0xfffffc00) == 0xdc00;
				if (is_high_surrogate && is_low_surrogate)
				{
					container.push_back((prev << 10) + curr - 0x35fdc00);
					++iter;
					continue;
				}
			}
			throw std::invalid_argument("需要转换编码的字符串不是有效的字符串");
		}
		else
		{
			container.push_back(prev);
		}
	}

	container.shrink_to_fit();
	return container;
}


// UTF-16/宽字符转换成UTF-8/窄字符的std::vector<char>
// charT wchar_t 或 char16_t
// s 需要转换的字符串
// n 需要转换的字符串的字符数, 不能超过int的最大值
// CodePage 目标字符编码
template <typename charT>
static std::vector<char> to_mbs(const charT *s, int n, UINT CodePage)
{
	if (n <= 0)
	{
		return std::vector<char>();
	}

	constexpr auto iswchar = std::is_same<charT, wchar_t>::value;
	constexpr auto ischar16 = std::is_same<charT, char16_t>::value;
	static_assert(iswchar || ischar16, "The input container must be a wide-character or UTF-16 string.");

	auto wstr = reinterpret_cast<const wchar_t *>(s);
	auto length = ::WideCharToMultiByte(CodePage, 0, wstr, n, nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		throw std::invalid_argument("需要转换编码的字符串不是有效的字符串");
	}

	std::vector<char> buffer(length);
	::WideCharToMultiByte(CodePage, 0, wstr, n, buffer.data(), static_cast<int>(buffer.size()), nullptr, nullptr);

	return buffer;
}


extios::codecvtor<extios::charset::multibyte, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>())
{
}


std::vector<char> extios::to_multibyte_buffer(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_UTF8);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_UTF8);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const std::string &text)
{
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::string extios::to_multibyte(const char *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::string &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_multibyte_buffer(const wchar_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const std::wstring &text)
{
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::string extios::to_multibyte(const wchar_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::wstring &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_multibyte_buffer(const char16_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const std::u16string &text)
{
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::string extios::to_multibyte(const char16_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::u16string &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_multibyte_buffer(const char32_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<std::vector<char16_t>>(s, n);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<std::vector<char16_t>>(s, n);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const std::u32string &text)
{
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_ACP);
}


std::string extios::to_multibyte(const char32_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::u32string &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char *s, unsigned int n, bool isutf8)
{
	throw_if_string_too_long(n);
	return ::to_utf16<wchar_t>(s, n, isutf8 ? CP_UTF8 : CP_ACP);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_utf16<wchar_t>(s, n, CP_ACP);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_utf16<wchar_t>(s, n, CP_UTF8);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::string &text, bool isutf8)
{
	throw_if_string_too_long(text.size());
	return ::to_utf16<wchar_t>(text.c_str(), static_cast<int>(text.size()), isutf8 ? CP_UTF8 : CP_ACP);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_utf16<wchar_t>(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_utf16<wchar_t>(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::wstring extios::to_widechar(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_widechar_buffer(s, n, isutf8);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(cvtor, s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(cvtor, s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const std::string &text, bool isutf8)
{
	auto buffer = to_widechar_buffer(text, isutf8);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text)
{
	auto buffer = to_widechar_buffer(cvtor, text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text)
{
	auto buffer = to_widechar_buffer(cvtor, text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char16_t *s, unsigned int n)
{
	return std::vector<wchar_t>(s, s + n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<wchar_t>(s, s + n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::u16string &text)
{
	return std::vector<wchar_t>(text.begin(), text.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<wchar_t>(text.begin(), text.end());
}


std::wstring extios::to_widechar(const char16_t *s, unsigned int n)
{
	return std::wstring(s, s + n);
}


std::wstring extios::to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::wstring(s, s + n);
}


std::wstring extios::to_widechar(const std::u16string &text)
{
	return std::wstring(text.begin(), text.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	return std::wstring(text.begin(), text.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char32_t *s, unsigned int n)
{
	return ::to_utf16<std::vector<wchar_t>>(s, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::vector<wchar_t>>(s, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::u32string &text)
{
	return ::to_utf16<std::vector<wchar_t>>(text.c_str(), text.size());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::vector<wchar_t>>(text.c_str(), text.size());
}


std::wstring extios::to_widechar(const char32_t *s, unsigned int n)
{
	return ::to_utf16<std::wstring>(s, n);
}


std::wstring extios::to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::wstring>(s, n);
}


std::wstring extios::to_widechar(const std::u32string &text)
{
	return ::to_utf16<std::wstring>(text.c_str(), text.size());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::wstring>(text.c_str(), text.size());
}


std::vector<char> extios::to_utf8_buffer(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_ACP);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_ACP);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const std::string &text)
{
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_ACP);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_ACP);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::string extios::to_utf8(const char *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::string &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_utf8_buffer(const wchar_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const std::wstring &text)
{
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::string extios::to_utf8(const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::wstring &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_utf8_buffer(const char16_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_mbs(s, n, CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const std::u16string &text)
{
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_mbs(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::string extios::to_utf8(const char16_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::u16string &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_utf8_buffer(const char32_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<std::vector<char16_t>>(s, n);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<std::vector<char16_t>>(s, n);
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const std::u32string &text)
{
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
	return ::to_mbs(buffer.data(), static_cast<int>(buffer.size()), CP_UTF8);
}


std::string extios::to_utf8(const char32_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::u32string &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const char *s, unsigned int n, bool isutf8)
{
	throw_if_string_too_long(n);
	return ::to_utf16<char16_t>(s, n, isutf8 ? CP_UTF8 : CP_ACP);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_utf16<char16_t>(s, n, CP_ACP);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::to_utf16<char16_t>(s, n, CP_UTF8);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::string &text, bool isutf8)
{
	throw_if_string_too_long(text.size());
	return ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), isutf8 ? CP_UTF8 : CP_ACP);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_ACP);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
}


std::u16string extios::to_utf16(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_utf16_buffer(s, n, isutf8);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const std::string &text, bool isutf8)
{
	auto buffer = to_utf16_buffer(text, isutf8);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const wchar_t *s, unsigned int n)
{
	return std::vector<char16_t>(s, s + n);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<char16_t>(s, s + n);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::wstring &text)
{
	return std::vector<char16_t>(text.begin(), text.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<char16_t>(text.begin(), text.end());
}


std::u16string extios::to_utf16(const wchar_t *s, unsigned int n)
{
	return std::u16string(s, s + n);
}


std::u16string extios::to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::u16string(s, s + n);
}


std::u16string extios::to_utf16(const std::wstring &text)
{
	return std::u16string(text.begin(), text.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return std::u16string(text.begin(), text.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const char32_t *s, unsigned int n)
{
	return ::to_utf16<std::vector<char16_t>>(s, n);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::vector<char16_t>>(s, n);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::u32string &text)
{
	return ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::vector<char16_t>>(text.c_str(), text.size());
}


std::u16string extios::to_utf16(const char32_t *s, unsigned int n)
{
	return ::to_utf16<std::u16string>(s, n);
}


std::u16string extios::to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::u16string>(s, n);
}


std::u16string extios::to_utf16(const std::u32string &text)
{
	return ::to_utf16<std::u16string>(text.c_str(), text.size());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf16<std::u16string>(text.c_str(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const char *s, unsigned int n, bool isutf8)
{
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, isutf8 ? CP_UTF8 : CP_ACP);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_ACP);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	auto buffer = ::to_utf16<char16_t>(s, n, CP_UTF8);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const std::string &text, bool isutf8)
{
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), isutf8 ? CP_UTF8 : CP_ACP);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_ACP);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	auto buffer = ::to_utf16<char16_t>(text.c_str(), static_cast<int>(text.size()), CP_UTF8);
	return ::to_utf32<std::vector<char32_t>>(buffer.data(), buffer.size());
}


std::u32string extios::to_utf32(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_utf32_buffer(s, n, isutf8);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(cvtor, s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(cvtor, s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const std::string &text, bool isutf8)
{
	auto buffer = to_utf32_buffer(text, isutf8);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text)
{
	auto buffer = to_utf32_buffer(cvtor, text);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text)
{
	auto buffer = to_utf32_buffer(cvtor, text);
	return std::u32string(buffer.begin(), buffer.end());
}


std::vector<char32_t> extios::to_utf32_buffer(const wchar_t *s, unsigned int n)
{
	return ::to_utf32<std::vector<char32_t>>(s, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::vector<char32_t>>(s, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const std::wstring &text)
{
	return ::to_utf32<std::vector<char32_t>>(text.c_str(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::vector<char32_t>>(text.c_str(), text.size());
}


std::u32string extios::to_utf32(const wchar_t *s, unsigned int n)
{
	return ::to_utf32<std::u32string>(s, n);
}


std::u32string extios::to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::u32string>(s, n);
}


std::u32string extios::to_utf32(const std::wstring &text)
{
	return ::to_utf32<std::u32string>(text.c_str(), text.size());
}


std::u32string extios::to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::u32string>(text.c_str(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const char16_t *s, unsigned int n)
{
	return ::to_utf32<std::vector<char32_t>>(s, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::vector<char32_t>>(s, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const std::u16string &text)
{
	return ::to_utf32<std::vector<char32_t>>(text.c_str(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::vector<char32_t>>(text.c_str(), text.size());
}


std::u32string extios::to_utf32(const char16_t *s, unsigned int n)
{
	return ::to_utf32<std::u32string>(s, n);
}


std::u32string extios::to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::u32string>(s, n);
}


std::u32string extios::to_utf32(const std::u16string &text)
{
	return ::to_utf32<std::u32string>(text.c_str(), text.size());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::to_utf32<std::u32string>(text.c_str(), text.size());
}


#else // !_MSC_VER

#include <iconv.h> // iconv_open iconv iconv_close
#include <cstring> // std::strerror
#include <cerrno> // cerrno

struct extios::codecvtor_base::member_data
{
	iconv_t cd;

	member_data(const char *fromcode, const char *tocode);
	~member_data(void);
	member_data(const member_data &) = delete;
	member_data(member_data &&) = delete;
	member_data & operator=(const member_data &) = delete;
	member_data & operator=(member_data &&) = delete;
};


extios::codecvtor_base::member_data::member_data(const char *fromcode, const char *tocode)
	: cd(::iconv_open(tocode, fromcode))
{
	if (cd == reinterpret_cast<iconv_t>(-1))
	{
		throw std::invalid_argument(std::strerror(errno));
	}
}


extios::codecvtor_base::member_data::~member_data(void)
{
	if (cd != reinterpret_cast<iconv_t>(-1))
	{
		::iconv_close(cd);
	}
}


void * extios::codecvtor_base::handle(void) const noexcept
{
	return m_data->cd;
}


extios::codecvtor<extios::charset::multibyte, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-32"))
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-8"))
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-16"))
{
}


extios::codecvtor<extios::charset::multibyte, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-32"))
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-8"))
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-8"))
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-16"))
{
}


extios::codecvtor<extios::charset::widechar, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-8"))
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-16"))
{
}


extios::codecvtor<extios::charset::utf8, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-8", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-16", "UTF-8"))
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-16", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-16", "UTF-8"))
{
}


extios::codecvtor<extios::charset::utf16, extios::charset::utf32>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-16", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::multibyte>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-8"))
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::widechar>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-32"))
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::utf8>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-8"))
{
}


extios::codecvtor<extios::charset::utf32, extios::charset::utf16>::codecvtor(void)
	: codecvtor_base(std::make_shared<member_data>("UTF-32", "UTF-16"))
{
}


static std::size_t iconvert(iconv_t cd, char *inbuf, std::size_t inbytes, char *outbuf, std::size_t outbytes)
{
	if (::iconv(cd, &inbuf, &inbytes, &outbuf, &outbytes) == static_cast<std::size_t>(-1))
	{
		throw std::invalid_argument(std::strerror(errno));
	}
	return outbytes;
}


template <typename OutputCharType, typename InputCharType, typename Convertor>
static std::vector<OutputCharType> convert_to(const Convertor &cvtor, const InputCharType *s, std::size_t n, std::size_t outputsize)
{
	using input_buffer_type = typename std::vector<InputCharType>;
	using output_buffer_type = typename std::vector<OutputCharType>;

	if (n == 0)
	{
		return output_buffer_type();
	}

	throw_if_cvtor_null(cvtor);

	input_buffer_type inbuf(s, s + n);
	output_buffer_type outbuf(outputsize + 1);

	auto pinbuf = reinterpret_cast<char *>(inbuf.data());
	auto poutbuf = reinterpret_cast<char *>(outbuf.data());
	auto inbytes = sizeof(InputCharType) * inbuf.size();
	auto outbytes = sizeof(OutputCharType) * outbuf.size();

	auto length = iconvert(cvtor.handle(), pinbuf, inbytes, poutbuf, outbytes);
	if constexpr (std::is_same<OutputCharType, char16_t>::value || std::is_same<OutputCharType, char32_t>::value)
	{
		outbuf.erase(outbuf.begin());
	}
	outbuf.resize(outbuf.size() - length / sizeof(OutputCharType));
	outbuf.shrink_to_fit();

	return outbuf;
}


std::vector<char> extios::to_multibyte_buffer(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return std::vector<char>(s, s + n);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return std::vector<char>(s, s + n);
}


std::vector<char> extios::to_multibyte_buffer(const std::string &text)
{
	throw_if_string_too_long(text.size());
	return std::vector<char>(text.begin(), text.end());
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return std::vector<char>(text.begin(), text.end());
}


std::string extios::to_multibyte(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return std::string(s, s + n);
}


std::string extios::to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return std::string(s, s + n);
}


std::string extios::to_multibyte(const std::string &text)
{
	throw_if_string_too_long(text.size());
	return std::string(text.begin(), text.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return std::string(text.begin(), text.end());
}


std::vector<char> extios::to_multibyte_buffer(const wchar_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::widechar, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const std::wstring &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::widechar, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text)
{
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_multibyte(const wchar_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::wstring &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_multibyte_buffer(const char16_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::utf16, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const std::u16string &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::utf16, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text)
{
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_multibyte(const char16_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::u16string &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_multibyte_buffer(const char32_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::utf32, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_multibyte_buffer(const std::u32string &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::utf32, charset::multibyte> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text)
{
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_multibyte(const char32_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n)
{
	auto buffer = to_multibyte_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const std::u32string &text)
{
	auto buffer = to_multibyte_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text)
{
	auto buffer = to_multibyte_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char *s, unsigned int n, bool)
{
	throw_if_string_too_long(n);
	codecvtor<charset::multibyte, charset::widechar> cvtor;
	return ::convert_to<wchar_t>(cvtor, s, n, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::convert_to<wchar_t>(cvtor, s, n, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return ::convert_to<wchar_t>(cvtor, s, n, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::string &text, bool)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::multibyte, charset::widechar> cvtor;
	return ::convert_to<wchar_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text)
{
	throw_if_string_too_long(text.size());
	return ::convert_to<wchar_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text)
{
	throw_if_string_too_long(text.size());
	return ::convert_to<wchar_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::wstring extios::to_widechar(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_widechar_buffer(s, n, isutf8);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(cvtor, s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(cvtor, s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const std::string &text, bool isutf8)
{
	auto buffer = to_widechar_buffer(text, isutf8);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text)
{
	auto buffer = to_widechar_buffer(cvtor, text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text)
{
	auto buffer = to_widechar_buffer(cvtor, text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char16_t *s, unsigned int n)
{
	codecvtor<charset::utf16, charset::widechar> cvtor;
	return ::convert_to<wchar_t>(cvtor, s, n, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n)
{
	return ::convert_to<wchar_t>(cvtor, s, n, n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::u16string &text)
{
	codecvtor<charset::utf16, charset::widechar> cvtor;
	return ::convert_to<wchar_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text)
{
	return ::convert_to<wchar_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::wstring extios::to_widechar(const char16_t *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_widechar_buffer(cvtor, s, n);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const std::u16string &text)
{
	auto buffer = to_widechar_buffer(text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text)
{
	auto buffer = to_widechar_buffer(cvtor, text);
	return std::wstring(buffer.begin(), buffer.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const char32_t *s, unsigned int n)
{
	return std::vector<wchar_t>(s, s + n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<wchar_t>(s, s + n);
}


std::vector<wchar_t> extios::to_widechar_buffer(const std::u32string &text)
{
	return std::vector<wchar_t>(text.begin(), text.end());
}


std::vector<wchar_t> extios::to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<wchar_t>(text.begin(), text.end());
}


std::wstring extios::to_widechar(const char32_t *s, unsigned int n)
{
	return std::wstring(s, s + n);
}


std::wstring extios::to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::wstring(s, s + n);
}


std::wstring extios::to_widechar(const std::u32string &text)
{
	return std::wstring(text.begin(), text.end());
}


std::wstring extios::to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return std::wstring(text.begin(), text.end());
}


std::vector<char> extios::to_utf8_buffer(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return std::vector<char>(s, s + n);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return std::vector<char>(s, s + n);
}


std::vector<char> extios::to_utf8_buffer(const std::string &text)
{
	throw_if_string_too_long(text.size());
	return std::vector<char>(text.begin(), text.end());
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return std::vector<char>(text.begin(), text.end());
}


std::string extios::to_utf8(const char *s, unsigned int n)
{
	throw_if_string_too_long(n);
	return std::string(s, s + n);
}


std::string extios::to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return std::string(s, s + n);
}


std::string extios::to_utf8(const std::string &text)
{
	throw_if_string_too_long(text.size());
	return std::string(text.begin(), text.end());
}


std::string extios::to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return std::string(text.begin(), text.end());
}


std::vector<char> extios::to_utf8_buffer(const wchar_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::widechar, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const std::wstring &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::widechar, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_utf8(const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::wstring &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_utf8_buffer(const char16_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::utf16, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const std::u16string &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::utf16, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_utf8(const char16_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::u16string &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char> extios::to_utf8_buffer(const char32_t *s, unsigned int n)
{
	throw_if_string_too_long(n);
	codecvtor<charset::utf32, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char>(cvtor, s, n, n * 4);
}


std::vector<char> extios::to_utf8_buffer(const std::u32string &text)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::utf32, charset::utf8> cvtor;
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::vector<char> extios::to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char>(cvtor, text.c_str(), text.size(), text.size() * 4);
}


std::string extios::to_utf8(const char32_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n)
{
	auto buffer = to_utf8_buffer(cvtor, s, n);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const std::u32string &text)
{
	auto buffer = to_utf8_buffer(text);
	return std::string(buffer.begin(), buffer.end());
}


std::string extios::to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text)
{
	auto buffer = to_utf8_buffer(cvtor, text);
	return std::string(buffer.begin(), buffer.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const char *s, unsigned int n, bool)
{
	throw_if_string_too_long(n);
	codecvtor<charset::multibyte, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::string &text, bool)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::multibyte, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::u16string extios::to_utf16(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_utf16_buffer(s, n, isutf8);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const std::string &text, bool isutf8)
{
	auto buffer = to_utf16_buffer(text, isutf8);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const wchar_t *s, unsigned int n)
{
	codecvtor<charset::widechar, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::wstring &text)
{
	codecvtor<charset::widechar, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::u16string extios::to_utf16(const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const std::wstring &text)
{
	auto buffer = to_utf16_buffer(text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::vector<char16_t> extios::to_utf16_buffer(const char32_t *s, unsigned int n)
{
	codecvtor<charset::utf32, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char16_t>(cvtor, s, n, n * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const std::u32string &text)
{
	codecvtor<charset::utf32, charset::utf16> cvtor;
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::vector<char16_t> extios::to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char16_t>(cvtor, text.c_str(), text.size(), text.size() * 2);
}


std::u16string extios::to_utf16(const char32_t *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n)
{
	auto buffer = to_utf16_buffer(cvtor, s, n);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const std::u32string &text)
{
	auto buffer = to_utf16_buffer(text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::u16string extios::to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text)
{
	auto buffer = to_utf16_buffer(cvtor, text);
	return std::u16string(buffer.begin(), buffer.end());
}


std::vector<char32_t> extios::to_utf32_buffer(const char *s, unsigned int n, bool)
{
	throw_if_string_too_long(n);
	codecvtor<charset::multibyte, charset::utf32> cvtor;
	return ::convert_to<char32_t>(cvtor, s, n, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char32_t>(cvtor, s, n, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(n);
	return ::convert_to<char32_t>(cvtor, s, n, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const std::string &text, bool)
{
	throw_if_string_too_long(text.size());
	codecvtor<charset::multibyte, charset::utf32> cvtor;
	return ::convert_to<char32_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char32_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text)
{
	throw_if_cvtor_null(cvtor);
	throw_if_string_too_long(text.size());
	return ::convert_to<char32_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::u32string extios::to_utf32(const char *s, unsigned int n, bool isutf8)
{
	auto buffer = to_utf32_buffer(s, n, isutf8);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(cvtor, s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(cvtor, s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const std::string &text, bool isutf8)
{
	auto buffer = to_utf32_buffer(text, isutf8);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text)
{
	auto buffer = to_utf32_buffer(cvtor, text);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text)
{
	auto buffer = to_utf32_buffer(cvtor, text);
	return std::u32string(buffer.begin(), buffer.end());
}


std::vector<char32_t> extios::to_utf32_buffer(const wchar_t *s, unsigned int n)
{
	return std::vector<char32_t>(s, s + n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<char32_t>(s, s + n);
}


std::vector<char32_t> extios::to_utf32_buffer(const std::wstring &text)
{
	return std::vector<char32_t>(text.begin(), text.end());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return std::vector<char32_t>(text.begin(), text.end());
}


std::u32string extios::to_utf32(const wchar_t *s, unsigned int n)
{
	return std::u32string(s, s + n);
}


std::u32string extios::to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return std::u32string(s, s + n);
}


std::u32string extios::to_utf32(const std::wstring &text)
{
	return std::u32string(text.begin(), text.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text)
{
	throw_if_cvtor_null(cvtor);
	return std::u32string(text.begin(), text.end());
}


std::vector<char32_t> extios::to_utf32_buffer(const char16_t *s, unsigned int n)
{
	codecvtor<charset::utf16, charset::utf32> cvtor;
	return ::convert_to<char32_t>(cvtor, s, n, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char32_t>(cvtor, s, n, n);
}


std::vector<char32_t> extios::to_utf32_buffer(const std::u16string &text)
{
	codecvtor<charset::utf16, charset::utf32> cvtor;
	return ::convert_to<char32_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::vector<char32_t> extios::to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text)
{
	throw_if_cvtor_null(cvtor);
	return ::convert_to<char32_t>(cvtor, text.c_str(), text.size(), text.size());
}


std::u32string extios::to_utf32(const char16_t *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n)
{
	auto buffer = to_utf32_buffer(cvtor, s, n);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const std::u16string &text)
{
	auto buffer = to_utf32_buffer(text);
	return std::u32string(buffer.begin(), buffer.end());
}


std::u32string extios::to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text)
{
	auto buffer = to_utf32_buffer(cvtor, text);
	return std::u32string(buffer.begin(), buffer.end());
}


#endif // _MSC_VER


extios::codecvtor_base::codecvtor_base(std::shared_ptr<member_data> &&data)
	: m_data(std::move(data))
{
}


extios::codecvtor_base::codecvtor_base(codecvtor_base &&data) noexcept
	: m_data(std::move(data.m_data))
{
}


extios::codecvtor_base & extios::codecvtor_base::operator=(codecvtor_base &&data) noexcept
{
	m_data = std::move(data.m_data);
	return *this;
}


bool extios::codecvtor_base::operator==(std::nullptr_t pointer) const noexcept
{
	return m_data == pointer;
}


bool extios::codecvtor_base::operator!=(std::nullptr_t pointer) const noexcept
{
	return m_data != pointer;
}


extios::codecvtor_base::operator bool(void) const noexcept
{
	return m_data != nullptr;
}
