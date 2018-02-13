#ifndef __EXTIOS_CODECVT_H__
#define __EXTIOS_CODECVT_H__

#include <memory> // std::shared_ptr
#include <vector> // std::vector
#include <string> // std::string std::wstring std::u16string std::u32string

#undef EXTIOSAPI
#ifdef _MSC_VER
#ifdef _EXTIOSDLL
#define EXTIOSAPI __declspec(dllexport)
#else // _EXTIOSDLL
#define EXTIOSAPI __declspec(dllimport)
#endif // _EXTIOSDLL
#else // _MSC_VER
#define EXTIOSAPI
#endif // _MSC_VER

// 扩展IO流
namespace extios
{
	// 字符集
	enum class charset
	{
		multibyte, // 本地字符集
		widechar, // 宽字符使用的字符集
		utf8, // UTF-8
		utf16, // UTF-16
		utf32 // UTF-32
	};

	// 编码转换基类, 只能用于extios库内部继承, 不能实例化对象
	class codecvtor_base
	{
	protected:
		struct member_data;

	public:
		codecvtor_base(const codecvtor_base &) = delete;
		EXTIOSAPI codecvtor_base(codecvtor_base &&) noexcept;

		codecvtor_base & operator=(const codecvtor_base &) = delete;
		EXTIOSAPI codecvtor_base & operator=(codecvtor_base &&) noexcept;

		EXTIOSAPI bool operator==(std::nullptr_t pointer) const noexcept;
		EXTIOSAPI bool operator!=(std::nullptr_t pointer) const noexcept;

		EXTIOSAPI operator bool(void) const noexcept;

		EXTIOSAPI void * handle(void) const noexcept;

	protected:
		EXTIOSAPI explicit codecvtor_base(std::shared_ptr<member_data> &&data);

	protected:
		std::shared_ptr<member_data> m_data;
	};

	// 编码转换类
	// 模板参数: from 待转换的字符集
	// 模板参数: to 转换后的字符集
	template <charset from, charset to> class codecvtor : public codecvtor_base
	{
	};

	// 用于本地字符集转换成宽字符字符集的编码转换类
	template <> class codecvtor<charset::multibyte, charset::widechar> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于本地字符集转换成UTF-8的编码转换类
	template <> class codecvtor<charset::multibyte, charset::utf8> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于本地字符集转换成UTF-16的编码转换类
	template <> class codecvtor<charset::multibyte, charset::utf16> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于本地字符集转换成UTF-32的编码转换类
	template <> class codecvtor<charset::multibyte, charset::utf32> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于宽字符字符集转换成本地字符集的编码转换类
	template <> class codecvtor<charset::widechar, charset::multibyte> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于宽字符字符集转换成UTF-8的编码转换类
	template <> class codecvtor<charset::widechar, charset::utf8> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于宽字符字符集转换成UTF-16的编码转换类
	template <> class codecvtor<charset::widechar, charset::utf16> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于宽字符字符集转换成UTF-32的编码转换类
	template <> class codecvtor<charset::widechar, charset::utf32> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-8转换成本地字符集的编码转换类
	template <> class codecvtor<charset::utf8, charset::multibyte> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-8转换成宽字符字符集的编码转换类
	template <> class codecvtor<charset::utf8, charset::widechar> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-8转换成UTF-16的编码转换类
	template <> class codecvtor<charset::utf8, charset::utf16> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-8转换成UTF-32的编码转换类
	template <> class codecvtor<charset::utf8, charset::utf32> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-16转换成本地字符集的编码转换类
	template <> class codecvtor<charset::utf16, charset::multibyte> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-16转换成宽字符字符集的编码转换类
	template <> class codecvtor<charset::utf16, charset::widechar> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-16转换成UTF-8的编码转换类
	template <> class codecvtor<charset::utf16, charset::utf8> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-16转换成UTF-32的编码转换类
	template <> class codecvtor<charset::utf16, charset::utf32> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-32转换成本地字符集的编码转换类
	template <> class codecvtor<charset::utf32, charset::multibyte> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-32转换成宽字符字符集的编码转换类
	template <> class codecvtor<charset::utf32, charset::widechar> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-32转换成UTF-8的编码转换类
	template <> class codecvtor<charset::utf32, charset::utf8> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// 用于UTF-32转换成UTF-16的编码转换类
	template <> class codecvtor<charset::utf32, charset::utf16> : public codecvtor_base
	{
	public:
		EXTIOSAPI codecvtor(void);
	};

	// UTF-8转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const char *s, unsigned int n);

	// UTF-8转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const std::string &text);

	// UTF-8转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text);

	// UTF-8转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const char *s, unsigned int n);

	// UTF-8转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const std::string &text);

	// UTF-8转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf8, charset::multibyte> &cvtor, const std::string &text);

	// 宽字符字符集转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const std::wstring &text);

	// 宽字符字符集转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text);

	// 宽字符字符集转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const std::wstring &text);

	// 宽字符字符集转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::widechar, charset::multibyte> &cvtor, const std::wstring &text);

	// UTF-16转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const char16_t *s, unsigned int n);

	// UTF-16转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const std::u16string &text);

	// UTF-16转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text);

	// UTF-16转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const char16_t *s, unsigned int n);

	// UTF-16转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const std::u16string &text);

	// UTF-16转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf16, charset::multibyte> &cvtor, const std::u16string &text);

	// UTF-32转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const char32_t *s, unsigned int n);

	// UTF-32转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const std::u32string &text);

	// UTF-32转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_multibyte_buffer(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text);

	// UTF-32转换成本地字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const char32_t *s, unsigned int n);

	// UTF-32转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成本地字符集
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const std::u32string &text);

	// UTF-32转换成本地字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 本地字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_multibyte(const codecvtor<charset::utf32, charset::multibyte> &cvtor, const std::u32string &text);

	// 本地字符集或UTF-8转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成宽字符字符集
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const std::string &text, bool isutf8);

	// 本地字符集转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text);

	// UTF-8转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text);

	// 本地字符集或UTF-8转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成宽字符字符集
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const std::string &text, bool isutf8);

	// 本地字符集转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::multibyte, charset::widechar> &cvtor, const std::string &text);

	// UTF-8转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf8, charset::widechar> &cvtor, const std::string &text);

	// UTF-16转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const char16_t *s, unsigned int n);

	// UTF-16转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成宽字符字符集
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const std::u16string &text);

	// UTF-16转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text);

	// UTF-16转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const char16_t *s, unsigned int n);

	// UTF-16转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成宽字符字符集
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const std::u16string &text);

	// UTF-16转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf16, charset::widechar> &cvtor, const std::u16string &text);

	// UTF-32转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const char32_t *s, unsigned int n);

	// UTF-32转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成宽字符字符集
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const std::u32string &text);

	// UTF-32转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<wchar_t> to_widechar_buffer(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text);

	// UTF-32转换成宽字符字符集
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const char32_t *s, unsigned int n);

	// UTF-32转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成宽字符字符集
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const std::u32string &text);

	// UTF-32转换成宽字符字符集
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: 宽字符字符集编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::wstring to_widechar(const codecvtor<charset::utf32, charset::widechar> &cvtor, const std::u32string &text);

	// 本地字符集转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const char *s, unsigned int n);

	// 本地字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n);

	// 本地字符集转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const std::string &text);

	// 本地字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text);

	// 本地字符集转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const char *s, unsigned int n);

	// 本地字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const char *s, unsigned int n);

	// 本地字符集转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const std::string &text);

	// 本地字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::multibyte, charset::utf8> &cvtor, const std::string &text);

	// 宽字符字符集转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const std::wstring &text);

	// 宽字符字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text);

	// 宽字符字符集转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const std::wstring &text);

	// 宽字符字符集转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::widechar, charset::utf8> &cvtor, const std::wstring &text);

	// UTF-16转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const std::u16string &text);

	// UTF-16转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text);

	// UTF-16转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const std::u16string &text);

	// UTF-16转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::utf16, charset::utf8> &cvtor, const std::u16string &text);

	// UTF-32转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const std::u32string &text);

	// UTF-32转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char> to_utf8_buffer(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text);

	// UTF-32转换成UTF-8
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-8
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const std::u32string &text);

	// UTF-32转换成UTF-8
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-8编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::string to_utf8(const codecvtor<charset::utf32, charset::utf8> &cvtor, const std::u32string &text);

	// 本地字符集或UTF-8转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成UTF-16
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const std::string &text, bool isutf8);

	// 本地字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text);

	// UTF-8转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text);

	// 本地字符集或UTF-8转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成UTF-16
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const std::string &text, bool isutf8);

	// 本地字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::multibyte, charset::utf16> &cvtor, const std::string &text);

	// UTF-8转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::utf8, charset::utf16> &cvtor, const std::string &text);

	// 宽字符字符集转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-16
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const std::wstring &text);

	// 宽字符字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text);

	// 宽字符字符集转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-16
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const std::wstring &text);

	// 宽字符字符集转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::widechar, charset::utf16> &cvtor, const std::wstring &text);
	
	// UTF-32转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-16
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const std::u32string &text);

	// UTF-32转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char16_t> to_utf16_buffer(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text);

	// UTF-32转换成UTF-16
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const char32_t *s, unsigned int n);

	// UTF-32转换成UTF-16
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const std::u32string &text);

	// UTF-32转换成UTF-16
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-16编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u16string to_utf16(const codecvtor<charset::utf32, charset::utf16> &cvtor, const std::u32string &text);

	// 本地字符集或UTF-8转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成UTF-32
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const std::string &text, bool isutf8);

	// 本地字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text);

	// UTF-8转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text);

	// 本地字符集或UTF-8转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const char *s, unsigned int n, bool isutf8);

	// 本地字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const char *s, unsigned int n);

	// UTF-8转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字节数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const char *s, unsigned int n);

	// 本地字符集或UTF-8转换成UTF-32
	// 参数: text 字符串对象
	// 参数: isutf8 true代表传入字符串是UTF-8, false代表本地字符集
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const std::string &text, bool isutf8);

	// 本地字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::multibyte, charset::utf32> &cvtor, const std::string &text);

	// UTF-8转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::utf8, charset::utf32> &cvtor, const std::string &text);

	// 宽字符字符集转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-32
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const std::wstring &text);

	// 宽字符字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text);

	// 宽字符字符集转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const wchar_t *s, unsigned int n);

	// 宽字符字符集转换成UTF-32
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const std::wstring &text);

	// 宽字符字符集转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::widechar, charset::utf32> &cvtor, const std::wstring &text);

	// UTF-16转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-32
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const std::u16string &text);

	// UTF-16转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串缓冲
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::vector<char32_t> to_utf32_buffer(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text);

	// UTF-16转换成UTF-32
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: s 字符串首地址
	// 参数: n 字符串的字符数
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const char16_t *s, unsigned int n);

	// UTF-16转换成UTF-32
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const std::u16string &text);

	// UTF-16转换成UTF-32
	// 参数: cvtor 转换类对象
	// 参数: text 字符串对象
	// 返回值: UTF-32编码的字符串
	// 异常: std::length_error 需要转换编码的字符串过长
	// 异常: std::invalid_argument 当cvtor是空对象时抛出异常或者当输入数据不是有效的字符串时抛出异常
	EXTIOSAPI std::u32string to_utf32(const codecvtor<charset::utf16, charset::utf32> &cvtor, const std::u16string &text);
}

#endif // !__EXTIOS_CODECVT_H__
