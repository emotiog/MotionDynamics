#ifndef __DEFINE_H_
#define __DEFINE_H_

#include <fstream>
#include <algorithm>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <string>
#include <map>				// hierarchical model
#include <unordered_map>	// AMC key frame
#include <sstream>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

#ifdef _UNICODE
namespace std
{
	typedef basic_string<wchar_t>			tstring;
	typedef basic_istream<wchar_t>			tistream;
	typedef basic_ostream<wchar_t>			tostream;
	typedef basic_fstream<wchar_t>			tfstream;
	typedef basic_ifstream<wchar_t>			tifstream;
	typedef basic_ofstream<wchar_t>			tofstream;
	typedef basic_stringstream<wchar_t>		tstringstream;
	typedef basic_istringstream<wchar_t>	tistringstream;
	typedef basic_ostringstream<wchar_t>	tostringstream;
}
#else
namespace std
{
	typedef basic_string<char>				tstring;
	typedef basic_istream<char>				tistream;
	typedef basic_ostream<char>				tostream;
	typedef basic_fstream<char>				tfstream;
	typedef basic_ifstream<char>			tifstream;
	typedef basic_ofstream<char>			tofstream;
	typedef basic_stringstream<char>		tstringstream;
	typedef basic_istringstream<char>		tistringstream;
	typedef basic_ostringstream<char>		tostringstream;
}
#endif

namespace NAnimation
{
	typedef std::vector<std::tstring> TStringArray;

	typedef CVector<double, 3>		Vector3d;
	typedef CVector<float,	3>		Vector3f;
	typedef CMatrix<double,	4, 4>	Matrix4x4d;
	typedef CMatrix<float,	4, 4>	Matrix4x4f;

	typedef Quaternion<float> Quaternionf;
	typedef std::vector< Quaternionf > QuaternionfArray;

	typedef Quaternion<double> Quaterniond;
	typedef std::vector< Quaterniond > QuaterniondArray;

}

#ifndef _UNICODE
#define ToUpper ToUpperA
#define ToLower ToLowerA
#define ToBackslash ToBackslashA
#define ToSlash ToSlashA
#else
#define ToUpper ToUpperW
#define ToLower ToLowerW
#define ToBackslash ToBackslashW
#define ToSlash ToSlashW
#endif


#endif