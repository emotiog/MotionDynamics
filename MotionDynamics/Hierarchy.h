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
		// ���ῡ �ʿ��ϳ� ������ �κп� ���Ͽ� ����մϴ�.
		std::map<std::wstring, bool>	mBodyMap;
		std::map<std::wstring, bool>	mEndeffecters;
		std::map<std::wstring, bool>	mSkipped;
		std::map<std::wstring, 
			std::vector<std::wstring>>	mHierarchy;

		std::vector<std::wstring>		mBodyName;
		std::vector<std::wstring>		mCutName;	// �߸� �ֵ�..

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