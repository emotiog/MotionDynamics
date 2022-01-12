#include "StdAfx.h"
#include "Hierarchy.h"


namespace NAnimation
{
	CHierarchy::CHierarchy(void)
	{
		mBodyName.push_back(_T("root"));
		mBodyName.push_back(_T("lhipjoint"));
		mBodyName.push_back(_T("lfemur"));
		mBodyName.push_back(_T("ltibia"));
		mBodyName.push_back(_T("rhipjoint"));
		mBodyName.push_back(_T("rfemur"));
		mBodyName.push_back(_T("rtibia"));
		mBodyName.push_back(_T("thorax"));
		mBodyName.push_back(_T("upperneck"));
		mBodyName.push_back(_T("lclavicle"));
		mBodyName.push_back(_T("lradius"));
		mBodyName.push_back(_T("lhumerus"));
		mBodyName.push_back(_T("rclavicle"));
		mBodyName.push_back(_T("rhumerus"));
		mBodyName.push_back(_T("rradius"));

		mCutName.push_back(_T("head"));
		mCutName.push_back(_T("rhand"));
		mCutName.push_back(_T("lhand"));
		mCutName.push_back(_T("lthumb"));
		mCutName.push_back(_T("rthumb"));
		mCutName.push_back(_T("lfingers"));
		mCutName.push_back(_T("rfingers"));
		mCutName.push_back(_T("lfoot"));
		mCutName.push_back(_T("rfoot"));
		mCutName.push_back(_T("ltoes"));
		mCutName.push_back(_T("rtoes"));
		mCutName.push_back(_T("lwrist"));
		mCutName.push_back(_T("rwrist"));

		mEndeffecters[_T("upperneck")]	= true;
		mEndeffecters[_T("ltibia")]		= true;
		mEndeffecters[_T("rtibia")]		= true;
		mEndeffecters[_T("lradius")]		= true;
		mEndeffecters[_T("rradius")]		= true;
	}


	CHierarchy::~CHierarchy(void)
	{
	}


	void CHierarchy::resize(unsigned int size)
	{
	}


	std::map<std::wstring, std::vector<std::wstring>> 
		CHierarchy::operator = (const CHierarchy& _rHierarchy)
	{
		std::map<std::wstring, std::vector<std::wstring>> result;
		
		mHierarchy	= _rHierarchy.mHierarchy;
		mSkipped	= _rHierarchy.mSkipped;

		return result;
	}


	void CHierarchy::refineHierarchy(void)
	{
		// 연결 리스트 삭제
		for (auto it = mHierarchy.begin(); it != mHierarchy.end(); ++it)
		{
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				// end node가 선정되어 전혀 필요 없는 경우
				if (it->second[i] == _T("rhand") ||
					it->second[i] == _T("lhand") ||
					it->second[i] == _T("lthumb") ||
					it->second[i] == _T("rthumb") ||
					it->second[i] == _T("lfingers") ||
					it->second[i] == _T("rfingers") ||
					it->second[i] == _T("ltoes") ||
					it->second[i] == _T("rtoes") ||
					it->second[i] == _T("lfoot") ||
					it->second[i] == _T("rfoot") ||
					it->second[i] == _T("lwrist") ||
					it->second[i] == _T("rwrist") ||
					it->second[i] == _T("head"))
				{
					it->second.erase(it->second.begin() + i);
					--i;
				}
			}
		}
		

		// hand - wrist를 end node로 할 것이니 자르기로 한다. ------------------
		auto it = mHierarchy.find(_T("rhand"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		
		it = mHierarchy.find(_T("lhand"));	
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------

		// head -- end effector로 쓰고 싶지만 머리 없는 모션도 있어서 삭제 -----
		it = mHierarchy.find(_T("head"));	
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------

		// thumb hand 보다 하위 노드이니 자르기로 한다 -------------------------
		it = mHierarchy.find(_T("lthumb"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		
		it = mHierarchy.find(_T("rthumb"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------
		
		// finger - hand 보다 하위 노드이니 자르기로 한다. ---------------------
		it = mHierarchy.find(_T("rfingers"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		
		it = mHierarchy.find(_T("lfingers"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------

		it = mHierarchy.find(_T("lfoot"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);

		it = mHierarchy.find(_T("rfoot"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------

		it = mHierarchy.find(_T("lwrist"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);

		it = mHierarchy.find(_T("rwrist"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------


		// toes - 디테일을 위해서는 중요하지만 foot을 end node로 삼기로 한다. --
		it = mHierarchy.find(_T("ltoes"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);

		it = mHierarchy.find(_T("rtoes"));
		if (it != mHierarchy.end())
			mHierarchy.erase(it);
		// ---------------------------------------------------------------------

	}


	void CHierarchy::registerHierarchy(std::map<std::wstring, std::vector<std::wstring>>* _pHierarchy)
	{

		for(unsigned int iter = 0 ; iter < mBodyName.size() ; iter ++)
		{
			auto it = _pHierarchy->find(mBodyName[iter]);
			if(it != _pHierarchy->end())
				mBodyMap[mBodyName[iter]]	= true;
		}

		// upperneck - 머리와 연결된 부분이니 자르지 않도록 하고 skip 되었다고 표시
		//auto it = _pHierarchy->find(_T("upperneck"));
		//if (it != _pHierarchy->end())
		//	mSkipped[_T("upperneck")] = true;

		auto it = _pHierarchy->find(_T("lowerneck"));
		if (it != _pHierarchy->end())
			mSkipped[_T("lowerneck")] = true;

		//it = _pHierarchy->find(_T("lradius"));
		//if (it != _pHierarchy->end())
		//	mSkipped[_T("lradius")] = true;
		//
		//it = _pHierarchy->find(_T("rradius"));
		//if (it != _pHierarchy->end())
		//	mSkipped[_T("rradius")] = true;

		it = _pHierarchy->find(_T("upperback"));
		if (it != _pHierarchy->end())
			mSkipped[_T("upperback")] = true;

		it = _pHierarchy->find(_T("lowerback"));
		if (it != _pHierarchy->end())
			mSkipped[_T("lowerback")] = true;

		mHierarchy = *_pHierarchy;
	}

	
	// find 재정의로 생각하면 된다
	// find로 어떤 값을 찾았는데 그 하위의 값이 내가 지우고자 하는 값일 경우
	// 그의 제자(혹은 자손)을 물려준다.
	// 그 자손 또한 삭제하고자 하는 값일 경우 또 그의 자손을 물려주어 결국
	// 내가 mBodyMap에서 필요한 것만 가져오고자 한다.
	std::map<std::wstring, std::vector<std::wstring>>::iterator CHierarchy::find(
		std::wstring _str)
	{
		// 값이 존재하는가?
		// 전혀 쓸모 없거나, 연결에 필요한 값들이 존재
		// 연결에 필요한 값인가?
		

		//while(!vaildData(_str))
		//{// 유효하지 않은 값이라면, 다음 값으로 검색
		//	auto it = mHierarchy.find(_str);
		//	
		//	if( it != mHierarchy.end())
		//		_str = it->second[0];
		//}

		//return mHierarchy.find(_str);
		do
		{// 하위 노드로 연결 - 단일 직선들이기 때문에 바로 자손으로 연결 가능
			auto it = mHierarchy.find(_str);

			// 내가 원하는 값인가?
			if(vaildData(_str))
				return it;

			_str = it->second[0];
		}while(mHierarchy.find(_str) != mHierarchy.end());

		// 필요하지 않은 값들로 값이 끝났을 경우. 혹은 없을 경우
		return mHierarchy.end();

	}


	// Vaild data ---------------------------------------------------------------
	//	본인이 BodyMap에 등록한 것 + 자손이 연결이 확신가능?
	bool CHierarchy::vaildData(const std::wstring& _rStr) const
	{
		auto it = mBodyMap.find(_rStr);

		if( it == mBodyMap.end())
			return false;

		return it->second;
	}

	
	bool CHierarchy::isSkipped(const std::wstring& _rStr)
	{
		auto it = mSkipped.find(_rStr);

		if(it == mSkipped.end())
			return false;

		return it->second;
	}


	std::map<std::wstring, std::vector<std::wstring>>::iterator CHierarchy::end(void)
	{
		return mHierarchy.end();
	}


	std::map<std::wstring, std::vector<std::wstring>>::iterator CHierarchy::begin(void)
	{
		return mHierarchy.begin();
	}


	bool CHierarchy::hasName(const std::wstring& _rStr) const
	{
		for(unsigned int iter = 0 ; iter < mBodyName.size() ; iter ++)
		{
			if( _rStr == mBodyName[iter])
				return true;
		}

		return false;
	}


	bool CHierarchy::isCut(const std::wstring& _rStr) const
	{
		for(unsigned int iter = 0 ; iter < mCutName.size() ; iter ++)
		{
			if( _rStr == mCutName[iter])
				return true;
		}
		
		return false;
	}


	bool CHierarchy::isEndNode(const std::wstring& _rStr) const
	{
		auto it		= mEndeffecters.find(_rStr);

		if( it == mEndeffecters.end())
			return false;


		return it->second;
	}
}