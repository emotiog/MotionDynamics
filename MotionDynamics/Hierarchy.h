#ifndef __HIERARCHY_H_
#define __HIERARCHY_H_

#include <map>
#include <vector>
#include "define.h"

namespace NAnimation
{
	class CHierarchy
	{
	public:
		// 연결에 필요하나 삭제할 부분에 대하여 사용합니다.
		std::map<std::wstring, bool>	mBodyMap;
		std::map<std::wstring, bool>	mEndeffecters;
		std::map<std::wstring, bool>	mSkipped;
		std::map<std::wstring, 
			std::vector<std::wstring>>	mHierarchy;

		std::vector<std::wstring>		mBodyName;
		std::vector<std::wstring>		mCutName;	// 잘린 애들..

	public:
		CHierarchy(void);
		~CHierarchy(void);

	public:
		std::map<std::wstring, std::vector<std::wstring>> operator = (const CHierarchy& _rHierarchy);

		void registerHierarchy(std::map<std::wstring, std::vector<std::wstring>>* _pHierarchy);
		void refineHierarchy(void);
	
	public:
		void resize(unsigned int size);
		bool vaildData(const std::wstring& _rStr) const;
		bool isSkipped(const std::wstring& _rStr);
		std::map<std::wstring, std::vector<std::wstring>>::iterator find(std::wstring _str);
		bool hasName(const std::wstring& _rStr) const;
		bool isCut(const std::wstring& _rStr) const;
		bool isEndNode(const std::wstring& _rStr) const;

	public:
		std::map<std::wstring, std::vector<std::wstring>>::iterator end(void);
		std::map<std::wstring, std::vector<std::wstring>>::iterator begin(void);
	};
}

#endif