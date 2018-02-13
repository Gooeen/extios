#ifndef __EXTIOS_IOSTREAM_HPP__
#define __EXTIOS_IOSTREAM_HPP__

#include <iostream>
#include <string>

#undef EXTIOS_GLOBAL
#ifdef _MSC_VER
#ifdef _EXTIOSDLL
#define EXTIOS_GLOBAL __declspec(dllexport)
#else // _EXTIOSDLL
#define EXTIOS_GLOBAL __declspec(dllimport)
#endif // _EXTIOSDLL
#else // _MSC_VER
#define EXTIOS_GLOBAL
#endif // _MSC_VER

namespace extios
{
	template <typename charT, typename Traits = std::char_traits<charT>>
	class ext_basic_istream : public std::basic_istream<charT, Traits>
	{
	public:
		explicit ext_basic_istream(std::basic_streambuf<charT, Traits> *sb);
	};

	template <typename charT, typename Traits = std::char_traits<charT>>
	class ext_basic_ostream : public std::basic_ostream<charT, Traits>
	{
	public:
		explicit ext_basic_ostream(std::basic_streambuf<charT, Traits> *sb);
	};

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, bool &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, short &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, unsigned short &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, int &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, unsigned int &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, long &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, unsigned long &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, long long &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, unsigned long long &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, float &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, double &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, long double &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, void* &val);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, charT *s);

	template<typename charT, typename Traits, typename Alloc>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, std::basic_string<charT, Traits, Alloc> &s);

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, std::istream& (*pf)(std::istream&));

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, std::ios& (*pf)(std::ios&));

	template<typename charT, typename Traits>
	ext_basic_istream<charT, Traits> & operator>>(ext_basic_istream<charT, Traits> &istr, std::ios_base& (*pf)(std::ios_base&));

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, bool val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, short val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned short val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, int val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned int val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, long val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned long val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, long long val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned long long val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, float val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, double val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, long double val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, void* val);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, const charT *s);

	template<typename charT, typename Traits, typename Alloc>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, const std::basic_string<charT, Traits, Alloc> &s);

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ostream&(*pf)(std::ostream&));

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ios& (*pf)(std::ios&));

	template<typename charT, typename Traits>
	ext_basic_ostream<charT, Traits> & operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ios_base& (*pf)(std::ios_base&));

	extern "C" EXTIOS_GLOBAL ext_basic_istream<char> u8cin;
	extern "C" EXTIOS_GLOBAL ext_basic_istream<wchar_t> wcin;
	extern "C" EXTIOS_GLOBAL ext_basic_istream<char16_t> u16cin;
	extern "C" EXTIOS_GLOBAL ext_basic_istream<char32_t> u32cin;

	extern "C" EXTIOS_GLOBAL ext_basic_ostream<char> u8cout;
	extern "C" EXTIOS_GLOBAL ext_basic_ostream<wchar_t> wcout;
	extern "C" EXTIOS_GLOBAL ext_basic_ostream<char16_t> u16cout;
	extern "C" EXTIOS_GLOBAL ext_basic_ostream<char32_t> u32cout;
}

template <typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits>::ext_basic_istream(std::basic_streambuf<charT, Traits> *sb)
	: std::basic_istream<charT, Traits>(sb)
{
}

template <typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits>::ext_basic_ostream(std::basic_streambuf<charT, Traits> *sb)
	: std::basic_ostream<charT, Traits>(sb)
{
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, bool &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, short &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, unsigned short &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, int &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, unsigned int &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, long &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, unsigned long &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, long long &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, unsigned long long &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, float &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, double &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, long double &val)
{
	std::cin >> val;
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, void* &val)
{
	std::cin >> val;
	return istr;
}

#ifdef _MSC_VER

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, charT *s)
{
	std::operator>>(istr, s);
	return istr;
}

template<typename charT, typename Traits, typename Alloc>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, std::basic_string<charT, Traits, Alloc> &s)
{
	std::operator>>(istr, s);
	return istr;
}

#else // _MSC_VER

#include <cctype>

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, charT *s)
{
	using myis = ext_basic_istream<charT, Traits>;
	std::ios_base::iostate state = std::ios_base::goodbit;
	charT *str0 = s;

	try
	{
		std::streamsize count = 0 < istr.width() ? istr.width() : std::numeric_limits<std::streamsize>::max();
		typename myis::int_type meta = istr.rdbuf()->sgetc();
		for (; 0 < --count; meta = istr.rdbuf()->snextc())
		{
			if (Traits::eq_int_type(Traits::eof(), meta))
			{
				state |= std::ios_base::eofbit;
				break;
			}
			else if (std::isspace(Traits::to_char_type(meta)) || Traits::to_char_type(meta) == charT())
			{
				break;
			}
			else
			{
				*s++ = Traits::to_char_type(meta);
			}
		}
	}
	catch (...)
	{
		istr.setstate(std::ios_base::badbit);
	}

	*s = charT();
	istr.width(0);
	istr.setstate(s == str0 ? state | std::ios_base::failbit : state);
	return istr;
}

template<typename charT, typename Traits, typename Alloc>
extios::ext_basic_istream<charT, Traits> & extios::operator>>(ext_basic_istream<charT, Traits> &istr, std::basic_string<charT, Traits, Alloc> &s)
{
	using myis = ext_basic_istream<charT, Traits>;
	using mystr = std::basic_string<charT, Traits, Alloc>;
	using mysizt = typename mystr::size_type;

	std::ios_base::iostate state = std::ios_base::goodbit;
	bool ischanged = false;
	const typename myis::sentry isok(istr);

	s.erase();

	try
	{
		mysizt size = 0 < istr.width() && (mysizt)istr.width() < s.max_size() ? (mysizt)istr.width() : s.max_size();
		typename Traits::int_type meta = istr.rdbuf()->sgetc();

		for (; 0 < size; --size, meta = istr.rdbuf()->snextc())
		{
			if (Traits::eq_int_type(Traits::eof(), meta))
			{
				state |= std::ios_base::eofbit;
				break;
			}
			else if (std::isspace(Traits::to_char_type(meta)))
			{
				break;
			}
			else
			{
				s.append(1, Traits::to_char_type(meta));
				ischanged = true;
			}
		}
	}
	catch (...)
	{
		istr.setstate(std::ios_base::badbit);
	}

	istr.width(0);
	if (!ischanged)
	{
		state |= std::ios_base::failbit;
	}
	istr.setstate(state);
	return istr;
}

#endif // _MSC_VER

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits>& extios::operator>>(ext_basic_istream<charT, Traits> &istr, std::istream&(*pf)(std::istream&))
{
	(*pf)(std::cin);
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits>& extios::operator>>(ext_basic_istream<charT, Traits> &istr, std::ios&(*pf)(std::ios&))
{
	(*pf)(std::cin);
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_istream<charT, Traits>& extios::operator>>(ext_basic_istream<charT, Traits> &istr, std::ios_base &(*pf)(std::ios_base &))
{
	(*pf)(std::cin);
	return istr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, bool val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, short val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned short val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, int val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned int val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, long val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned long val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, long long val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, unsigned long long val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, float val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, double val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, long double val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, void* val)
{
	std::cout << val;
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, const charT *s)
{
	std::size_t length = 0;
	for (auto p = s; (*p) != charT(); ++p, ++length);
	ostr.write(s, length);
	return ostr;
}

template<typename charT, typename Traits, typename Alloc>
extios::ext_basic_ostream<charT, Traits> & extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, const std::basic_string<charT, Traits, Alloc> &s)
{
	ostr.write(s.c_str(), s.size());
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits>& extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ostream&(*pf)(std::ostream&))
{
	(*pf)(std::cout);
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits>& extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ios&(*pf)(std::ios&))
{
	(*pf)(std::cout);
	return ostr;
}

template<typename charT, typename Traits>
extios::ext_basic_ostream<charT, Traits>& extios::operator<<(ext_basic_ostream<charT, Traits> &ostr, std::ios_base &(*pf)(std::ios_base &))
{
	(*pf)(std::cout);
	return ostr;
}

#endif // __EXTIOS_IOSTREAM_HPP__
