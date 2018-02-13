#ifndef __EXTIOS_IOBUF_HPP__
#define __EXTIOS_IOBUF_HPP__

#include "extcodecvt.h"
#include <streambuf> // std::basic_streambuf
#include <iostream> // std::cout, std::cin
#include <cctype> // std::isspace

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

namespace extios
{
	namespace _hidden
	{
		template <typename charT>
		std::vector<charT> from_multibytes(const char *s, std::size_t n);

		template <>
		std::vector<char> from_multibytes<char>(const char *s, std::size_t n);

		template <>
		std::vector<wchar_t> from_multibytes<wchar_t>(const char *s, std::size_t n);

		template <>
		std::vector<char16_t> from_multibytes<char16_t>(const char *s, std::size_t n);

		template <>
		std::vector<char32_t> from_multibytes<char32_t>(const char *s, std::size_t n);
	}

	template <typename charT, typename traits = std::char_traits<charT>>
	class basic_inputbuf : public std::basic_streambuf<charT, traits>
	{
	public:
		using char_type = charT;
		using traits_type = traits;
		using int_type = typename traits_type::int_type;
		using pos_type = typename traits_type::pos_type;
		using off_type = typename traits_type::off_type;

	public:
		basic_inputbuf(void);
		basic_inputbuf(const basic_inputbuf &) = delete;
		basic_inputbuf(basic_inputbuf &&x);
		~basic_inputbuf(void);
		basic_inputbuf & operator=(const basic_inputbuf &) = delete;
		basic_inputbuf & operator=(basic_inputbuf &&x);

	protected:
		virtual int_type underflow(void) override;

	private:
		static constexpr std::size_t buffer_size = 32;
		static constexpr std::size_t last_buffer_size = 4;
		char_type * m_buffer;
		char_type * m_last_buffer;
		char_type * m_last_buffer_pointer;
	};

	template <typename charT, typename traits = std::char_traits<charT>>
	class basic_outputbuf : public std::basic_streambuf<charT, traits>
	{
	public:
		using char_type = charT;
		using traits_type = traits;
		using int_type = typename traits_type::int_type;
		using pos_type = typename traits_type::pos_type;
		using off_type = typename traits_type::off_type;

	public:
		basic_outputbuf(void) = default;
		basic_outputbuf(const basic_outputbuf &) = delete;
		basic_outputbuf(basic_outputbuf &&) = default;
		basic_outputbuf & operator=(const basic_outputbuf &) = delete;
		basic_outputbuf & operator=(basic_outputbuf &&) = default;

	protected:
		virtual std::streamsize xsputn(const char_type *s, std::streamsize n) override;
		virtual int_type overflow(int_type c) override;
	};
}

template<>
inline std::vector<char> extios::_hidden::from_multibytes(const char *s, std::size_t n)
{
	return to_utf8_buffer(s, static_cast<unsigned int>(n));
}

template<>
inline std::vector<wchar_t> extios::_hidden::from_multibytes(const char *s, std::size_t n)
{
	return to_widechar_buffer(s, static_cast<unsigned int>(n), false);
}

template<>
inline std::vector<char16_t> extios::_hidden::from_multibytes(const char *s, std::size_t n)
{
	return to_utf16_buffer(s, static_cast<unsigned int>(n), false);
}

template<>
inline std::vector<char32_t> extios::_hidden::from_multibytes(const char *s, std::size_t n)
{
	return to_utf32_buffer(s, static_cast<unsigned int>(n), false);
}

template<typename charT, typename traits>
inline extios::basic_inputbuf<charT, traits>::basic_inputbuf(void)
	: m_buffer(new char_type[buffer_size])
	, m_last_buffer(new char_type[last_buffer_size])
	, m_last_buffer_pointer(m_last_buffer)
{
	this->setg(m_buffer, m_buffer + buffer_size, m_buffer + buffer_size);
}

template<typename charT, typename traits>
inline extios::basic_inputbuf<charT, traits>::basic_inputbuf(basic_inputbuf &&x)
	: m_buffer(x.m_buffer)
	, m_last_buffer(x.m_last_buffer)
	, m_last_buffer_pointer(x.m_last_buffer_pointer)
{
	x.m_buffer = nullptr;
	x.m_last_buffer = nullptr;
	x.m_last_buffer_pointer = nullptr;
}

template<typename charT, typename traits>
inline extios::basic_inputbuf<charT, traits>::~basic_inputbuf(void)
{
	if (m_last_buffer != nullptr)
	{
		delete[] m_last_buffer;
	}
	if (m_buffer != nullptr)
	{
		delete[] m_buffer;
	}
}

template<typename charT, typename traits>
inline extios::basic_inputbuf<charT, traits> & extios::basic_inputbuf<charT, traits>::operator=(basic_inputbuf &&x)
{
	m_buffer = x.m_buffer;
	m_last_buffer = x.m_last_buffer;
	m_last_buffer_pointer = x.m_last_buffer_pointer;
	x.m_buffer = nullptr;
	x.m_last_buffer = nullptr;
	x.m_last_buffer_pointer = nullptr;
}

template<typename charT, typename traits>
inline typename extios::basic_inputbuf<charT, traits>::int_type extios::basic_inputbuf<charT, traits>::underflow(void)
{
	this->setg(m_buffer, m_buffer, m_buffer + buffer_size);

	for (auto p = m_last_buffer; p != m_last_buffer_pointer; ++p)
	{
		*(this->gptr()) = (*p);
		this->gbump(1);
	}
	m_last_buffer_pointer = m_last_buffer;

	constexpr std::size_t temp_size = 4;
	char temp[temp_size]{};
	char *p = temp;
	do
	{
		if (p - temp == temp_size)
		{
			break;
		}

		*p++ = static_cast<char>(std::cin.rdbuf()->sbumpc());
		if (traits_type::eq_int_type(p[-1], '\n') || traits_type::eq_int_type(p[-1], traits_type::eof()))
		{
			*(this->gptr()) = traits_type::to_char_type(p[-1]);
			this->gbump(1);
			break;
		}

		try
		{
			auto buffer = _hidden::from_multibytes<char_type>(temp, p - temp);
			if (p[-1] != '?' && buffer.back() == traits_type::to_char_type('?'))
			{
				throw std::invalid_argument("");
			}

			if (buffer.size() > static_cast<std::size_t>(this->egptr() - this->gptr()))
			{
				std::size_t i = 0;
				for (; i < last_buffer_size && i < buffer.size(); ++i)
				{
					m_last_buffer[i] = buffer[i];
				}
				m_last_buffer_pointer = m_last_buffer + i;
				break;
			}

			for (auto ch : buffer)
			{
				*(this->gptr()) = ch;
				this->gbump(1);
			}

			p = temp;
		}
		catch (const std::invalid_argument &)
		{
		}
	} while ((this->gptr() != this->egptr()));
	this->setg(m_buffer, m_buffer, this->gptr());
	return traits_type::to_int_type(*m_buffer);
}

template<typename charT, typename traits>
inline std::streamsize extios::basic_outputbuf<charT, traits>::xsputn(const char_type *s, std::streamsize n)
{
	if (n > std::numeric_limits<int>::max())
	{
		std::length_error("需要输出的字符串过长");
	}
	auto buffer = to_multibyte_buffer(s, static_cast<unsigned int>(n));
	std::cout.rdbuf()->sputn(buffer.data(), buffer.size());
	return n;
}

template<typename charT, typename traits>
inline typename extios::basic_outputbuf<charT, traits>::int_type extios::basic_outputbuf<charT, traits>::overflow(int_type c)
{
	auto ch = static_cast<char_type>(c);
	auto buffer = to_multibyte_buffer(&ch, 1);
	std::cout.rdbuf()->sputn(buffer.data(), buffer.size());
	return c;
}

#endif // !__EXTIOS_IOBUF_HPP__
