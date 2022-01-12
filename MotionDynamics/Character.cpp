#include "stdafx.h"
#include "Character.h"

namespace NAnimation
{
	ToUpperW::ToUpperW()
	{
	}

	wchar_t ToUpperW::operator()(wchar_t ch)
	{
		return static_cast<wchar_t>(towupper(ch));
	}
}