
#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "define.h"

namespace NAnimation
{
	class ToUpperW
	{
	public:
		ToUpperW();
		wchar_t operator()(wchar_t ch);
	};
}
#endif