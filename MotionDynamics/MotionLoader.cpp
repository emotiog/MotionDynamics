#include "stdafx.h"
#include "MotionLoader.h"


namespace NAnimation
{
	CMotionLoader::CMotionLoader(void)
	{
		mpBody	= NULL;

		mDegree	= true;
		mGlobalLengthMultiplier	= 1;	// 어자피 파일 로딩 값..
	}

	CMotionLoader::CMotionLoader(CSkeleton* _pBody)
	{
		mpBody	= _pBody;

		mDegree	= true;
		mGlobalLengthMultiplier	= 1;
	}


	CMotionLoader::~CMotionLoader(void)
	{
	}


	void CMotionLoader::clear(void)
	{
		mpBody->reset();
		mpBody	= NULL;
	}


	bool CMotionLoader::connectSkeleton(CSkeleton* _pBody)
	{
		if(_pBody == NULL)
			return false;

		if(mpBody == NULL)
			mpBody	= _pBody;
		else
		{	
			return false;
			//mpBody->reset();
			//mpBody	= _pBody;
		}

		return true;
	}



	void CMotionLoader::_skipEmptyChar(std::tistream& inStream)
	{
		TCHAR ch = _T('\0');

		while(inStream)
		{
			ch = inStream.peek();

			if(_T(' ') == ch || _T('\t') == ch)
				inStream.get();

			else if( _T('\r') == ch || _T('\n') == ch)
				inStream.get();

			else
				break;
		}
	}


	void CMotionLoader::_skipSpaceChar(std::tistream& inStream)
	{
		TCHAR ch = _T('\0');

		while(inStream)
		{
			ch = inStream.peek();

			if(_T(' ') == ch || _T('\t') == ch)
				inStream.get();

			else
				break;
		}
	}


	bool CMotionLoader::parseAsfVersion(std::tifstream& fin)
	{
		std::tstring sVersion;
		std::getline(fin, sVersion);
		//assert(sVersion == _T("1.10"));
		return true;
	}


	bool CMotionLoader::parseAsfName(std::tifstream& fin)
	{
		std::tstring sName;
		std::getline(fin, sName);

		mName = sName;
		//assert(sName == _T("VICON"));
		return true;
	}


	bool CMotionLoader::parseAsfUnits(std::tifstream& fin)
	{
		std::tstring sBuffer;

		fin >> sBuffer;
		assert(_T("mass") == sBuffer);
		fin >> mpBody->mBodyMass;

		fin >> sBuffer;
		assert(_T("length") == sBuffer);
		fin >> mGlobalLengthMultiplier;

		fin >> sBuffer;
		assert(_T("angle") == sBuffer);
		fin >> sBuffer;

		std::transform(sBuffer.begin(), sBuffer.end(), sBuffer.begin(), ToUpper());

		if (_tcsncmp(sBuffer.c_str(), _T("DEG"), 3) == 0)
			mDegree = true;
		else
			mDegree = false;

		return true;
	}


	bool CMotionLoader::parseAsfDocumentation(std::tifstream& fin)
	{
		std::tstring sBuffer;

		TCHAR ch = 0;
		while (fin)
		{
			_skipSpaceChar(fin);
			ch = fin.peek();
			if (_T(':') == ch || ch <= 0 || !fin || fin.eof())
				break;
			else
				std::getline(fin, sBuffer);
		}

		return true;
	}


	Vector3d CMotionLoader::_convertAngleToDegree(const Vector3d& _rDir)
	{
		Vector3d degreeDir;

		degreeDir.x = _rDir.x * PI_D / 180.0;
		degreeDir.y = _rDir.y * PI_D / 180.0;
		degreeDir.z = _rDir.z * PI_D / 180.0;

		return degreeDir;
	}


	bool CMotionLoader::parseAsfRoot(std::tifstream& fin)
	{
		CJoint asf_joint;
		asf_joint.setIndex( 0);
		asf_joint.setName(_T("root"));

		std::tstring sBuffer;

		int count = 0;
		while (count < 4)
		{
			fin >> sBuffer;
			if (_T("order") == sBuffer)
			{
				std::tstring sAmcOrder;
				std::getline(fin, sAmcOrder);

				parseAsfAmcOrder(sAmcOrder, asf_joint);
			}
			else if (_T("axis") == sBuffer)
			{
				std::tstring sAxis;
				fin >> sAxis;
				asf_joint.eRotOrder = GetEulerRotationTypeFromString(sAxis);
			}
			else if (_T("position") == sBuffer)
			{
				Vector3d position;
				fin >> position.x;
				fin >> position.y;
				fin >> position.z;

				asf_joint.setLocalJointPosition(position);
			}
			else if (_T("orientation") == sBuffer)
			{
				Vector3d dir;
				fin >> dir.x;
				fin >> dir.y;
				fin >> dir.z;

				asf_joint.setLocalJointAxis(dir);

				if (mDegree)
				{
					asf_joint.setLocalJointAxis(_convertAngleToDegree(dir)); 
				}
			}
			else
			{
				assert(false);
				return false;
			}

			count += 1;
		}

		if(!mpBody->emptyJoint())
		{
			mpBody->reset();
		}
		mpBody->attachJoint(&asf_joint);

		return true;
	}


	void CMotionLoader::parseAsfAmcOrder(const std::tstring& sAmcOrder, CJoint& asf_joint)
	{
		unsigned int index = 0;
		for (size_t i = 0; i < sAmcOrder.size(); ++i)
		{
			if (index >= 7)
				break;

			if (sAmcOrder[i] == _T(' ')  || sAmcOrder[i] == _T('\t') ||
				sAmcOrder[i] == _T('\r') || sAmcOrder[i] == _T('\n'))
				continue;
			else if (sAmcOrder[i] == _T('r') || sAmcOrder[i] == _T('R'))
			{
				if (sAmcOrder[i+1] == _T('x') || sAmcOrder[i+1] == _T('X'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("RX");
					index += 1;
					continue;
				}
				else if (sAmcOrder[i+1] == _T('y') || sAmcOrder[i+1] == _T('Y'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("RY");
					index += 1;
					continue;
				}
				else if (sAmcOrder[i+1] == _T('z') || sAmcOrder[i+1] == _T('Z'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("RZ");
					index += 1;
					continue;
				}
				else
					assert(false);
			}
			else if (sAmcOrder[i] == _T('t') || sAmcOrder[i] == _T('T'))
			{
				if (sAmcOrder[i+1] == _T('x') || sAmcOrder[i+1] == _T('X'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("TX");
					index += 1;
					continue;
				}
				else if (sAmcOrder[i+1] == _T('y') || sAmcOrder[i+1] == _T('Y'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("TY");
					index += 1;
					continue;
				}
				else if (sAmcOrder[i+1] == _T('z') || sAmcOrder[i+1] == _T('Z'))
				{
					i += 1;
					asf_joint.mDOF.mAmcOrder[index] = _T("TZ");
					index += 1;
					continue;
				}
				else
					assert(false);
			}
			else if (sAmcOrder[i] == _T('l') || sAmcOrder[i] == _T('L'))
			{
				asf_joint.mDOF.mAmcOrder[index] = _T("l");
				index += 1;
				continue;
			}
			else
				assert(false);
		}

		assert(0 == asf_joint.mDOF.mAmcOrderCount);
		asf_joint.mDOF.mAmcOrderCount = index;
	}


	bool CMotionLoader::parseAsfBoneData(std::tifstream& fin)
	{
		std::tstring sBuffer;
		while (fin)
		{
			_skipEmptyChar(fin);

			TCHAR ch = fin.peek();
			if (_T(':') == ch)
				break;
			else if (ch <= 0)
				break;
			else if (!fin || fin.eof())
				break;

			fin >> sBuffer;
			assert(_T("begin") == sBuffer);
			if (_T("begin") != sBuffer)
				return false;

			CJoint asf_joint;

			double direction[3] = { 0.0, 0.0, 0.0 };
		// joint 별로 length 를 가지고 있는 구조로 변환	

			while (fin)
			{
				fin >> sBuffer;
				if (!fin && fin.eof())
					break;
				else if (sBuffer == _T("end"))
					break;

				if (_T("id") == sBuffer)
				{
					unsigned int idex;
					assert(0xffffffff == asf_joint.getIndex());
					fin >> idex;

					asf_joint.setIndex(idex);

				}
				else if (_T("name") == sBuffer)
				{
					std::wstring name;
					assert(asf_joint.isName());
					fin >> name;

					asf_joint.setName(name);

					assert(!asf_joint.isName());
				}
				else if (_T("direction") == sBuffer)
				{
					fin >> direction[0];
					fin >> direction[1];
					fin >> direction[2];

					asf_joint.setLocalJointDirection(direction[0], direction[1], direction[2]);
				}
				else if (_T("length") == sBuffer)
				{
					double length;
					fin >> length;

					asf_joint.setLength(length);
				}
				else if (_T("axis") == sBuffer)
				{
					Vector3d dir;

					fin >> dir.x;
					fin >> dir.y;
					fin >> dir.z;

					asf_joint.setLocalJointAxis(dir);

					if (mDegree)
					{
						asf_joint.setLocalJointAxis(_convertAngleToDegree(dir)); 
					}

					std::tstring sAxisOrder;
					fin >> sAxisOrder;

					assert(sAxisOrder.size() == 3);

					asf_joint.eRotOrder = GetEulerRotationTypeFromString(sAxisOrder);
				}
				else if (_T("bodymass") == sBuffer)
				{
					double mass;
					fin >> mass;

					asf_joint.setMass(mass);
				}
				else if (_T("cofmass") == sBuffer)
				{
					//fin >> asf_joint.dCOM;
				}
				else if (_T("dof") == sBuffer)
				{
					std::tstring sAmcOrder;
					std::getline(fin, sAmcOrder);

					parseAsfAmcOrder(sAmcOrder, asf_joint);
				}
				else if (_T("limits") == sBuffer)
				{
					for (unsigned int i = 0; i < asf_joint.mDOF.mAmcOrderCount; ++i)
					{
						TCHAR ch = 0;
						fin >> ch;
						assert(ch == _T('('));

						std::tstring sMin;
						fin >> sMin;

						if (_tcsncmp(sMin.c_str(), _T("-inf"), 4) == 0)
							asf_joint.mDOF.dLimits[i][0] = -std::numeric_limits<float>::max();
						else if (_tcsncmp(sMin.c_str(), _T("inf"), 3) == 0)
							asf_joint.mDOF.dLimits[i][0] =  std::numeric_limits<float>::max();
						else
							asf_joint.mDOF.dLimits[i][0] = static_cast<float>(_tstof(sMin.c_str()));

						_skipSpaceChar(fin);

						std::tstring sMax;
						std::getline(fin, sMax, _T(')'));

						if (_tcsncmp(sMax.c_str(), _T("-inf"), 4) == 0)
							asf_joint.mDOF.dLimits[i][1] = -std::numeric_limits<float>::max();
						else if (_tcsncmp(sMax.c_str(), _T("inf"), 3) == 0)
							asf_joint.mDOF.dLimits[i][1] =  std::numeric_limits<float>::max();
						else
							asf_joint.mDOF.dLimits[i][1] = static_cast<float>(_tstof(sMax.c_str()));

						fin.get();

						if (mDegree)
						{
							asf_joint.mDOF.dLimits[i][0] = asf_joint.mDOF.dLimits[i][0] * PI_D / 180.0;
							asf_joint.mDOF.dLimits[i][1] = asf_joint.mDOF.dLimits[i][1] * PI_D / 180.0;
						}
					}
				}
				else
				{
					assert(false);
					return false;
				}
			}

			// Inch -> meter
			Vector3d pos;
			pos.x = ((direction[0] * asf_joint.getLength()) * 2.54)/ 100.0;
			pos.y = ((direction[1] * asf_joint.getLength()) * 2.54)/ 100.0;
			pos.z = ((direction[2] * asf_joint.getLength()) * 2.54)/ 100.0;

			asf_joint.setLocalJointPosition(pos);

			// Usually...
			assert(asf_joint.getIndex() == static_cast<unsigned int>(mpBody->getJointSize()));
			mpBody->attachJoint(&asf_joint);
			}

		return true;
	}


	bool CMotionLoader::parseAsfHierarchy(
		std::tifstream& fin,
		std::map< std::tstring, TStringArray >& hierarchy)
	{
		std::tstring sBuffer;
		fin >> sBuffer;

		assert(_T("begin") == sBuffer);
		if (_T("begin") != sBuffer)
			return false;

		while (fin)
		{
			std::tstring sParent;
			fin >> sParent;

			if (_T("end") == sParent)
				break;

			auto it = hierarchy.find(sParent);
			if (it != hierarchy.end())
				assert(false);

			hierarchy[sParent] = TStringArray();
			TStringArray& children = hierarchy[sParent];

			while (fin)
			{
				_skipSpaceChar(fin);
				TCHAR ch = fin.peek();
				if (_T('\r') == ch || _T('\n') == ch)
					break;

				std::tstring child;
				fin >> child;
				if (!fin || fin.eof() || child.empty())
					break;

				children.push_back(child);
			}
		}

		return true;
	}


	bool CMotionLoader::loadAcclaimSkeletonFile(const std::wstring& _rFilePath)
	{
		if(mpBody == NULL)
		{
			assert(L"acclocate pointer error");
			return false;
		}

		mFilePath	= _rFilePath;

		std::map<std::wstring, StringArray> hierarchy;

		std::tifstream fin;

		fin.open(_rFilePath.c_str());

		if(!fin.is_open())
		{
			assert(_T("Failed to open asf file\n"));
			return false;
		}

		TCHAR	ch;
		std::tstring	sBuffer;

		bool bResult = true;

		while(fin && bResult)
		{
			_skipEmptyChar(fin);

			ch = fin.get();

			if(!fin || fin.eof())
				break;
			else if( ch <= 0)
				break;

			if(_T('#') == ch)
			{
				std::getline(fin, sBuffer);
				continue;
			}
			else if(_T(':') == ch)
			{
				fin >> sBuffer;
				if (_T("version") == sBuffer)
				{
					if (!parseAsfVersion(fin))
						bResult = false;
				}
				else if (_T("name") == sBuffer)
				{
					if (!parseAsfName(fin))
						bResult = false;
				}
				else if (_T("units") == sBuffer)
				{
					if (!parseAsfUnits(fin))
						bResult = false;
				}
				else if (_T("documentation") == sBuffer)
				{
					if (!parseAsfDocumentation(fin))
						bResult = false;
				}
				else if (_T("root") == sBuffer)
				{
					if (!parseAsfRoot(fin))
						bResult = false;
				}
				else if (_T("bonedata") == sBuffer)
				{
					if (!parseAsfBoneData(fin))
						bResult = false;
				}
				else if (_T("hierarchy") == sBuffer)
				{
					if (!parseAsfHierarchy(fin, hierarchy))
						bResult = false;
				}
			}
		}
		

// 붙일 뼈 마디 수정 -----------------------------------------------
		// 모든 joint들은 이미 다 만들어진 상태 ->  서로 연결 고리만 없을 뿐.

		mHierarchy.registerHierarchy(&hierarchy);
		_bodySimplify(mHierarchy);
		mHierarchy.refineHierarchy();
//-----------------------------------------------------------------------------	
		mpBody->resetIndex();

		fin.clear();
		fin.close();

#ifdef _SKELETON
 		_connectByHierarchy(&mHierarchy);
		mpBody->centeralHierarchy();
#else
		mpBody->connectByHierarchy(hierarchy);
#endif

		if (!bResult)
		{
			assert(_T("Failed to load asf file\n"));
			return false;
		}

		_checkValidation();
		_setJointType(mHierarchy);
		return true;
	}


	void CMotionLoader::_bodySimplify(const CHierarchy& _rHierarchy)
	{
		for(unsigned int it = 0 ; it < mpBody->getJointSize() ; it++)
		{
			// string 으로 이름을 find 해서 
			// 내가 원하는 이름들만 불러온다?
			if(!_rHierarchy.hasName(mpBody->getIndex(it)->getName()))
			{
				mpBody->removeJoint(it);
				it--;
			}
		}
	}


	bool CMotionLoader::_checkValidation(void)
	{
		int onlyOneRoot = 0;
		for(size_t it_ = 0 ; it_ < mpBody->getJointSize() ; it_ ++)
		{
			if(!mpBody->isParent(it_))
			{
				onlyOneRoot ++;
			}
		}

		if(onlyOneRoot != 1)
		{
			assert(_T("Invalid ASF file format\n"));
			return false;
		}

		return true;
	}


	bool CMotionLoader::parseAmcKeyframe(std::tifstream& fin,
		std::vector<CAMCkeyframe>& motionSeq)
	{
		_skipEmptyChar(fin);

		TCHAR ch = 0;
		std::tstring sLine;

		while (fin)
		{
			ch = fin.peek();

			if (!fin || fin.eof())
				break;
			else if (ch <= 0)
				break;

			if (isdigit(ch))
				break;

			else
			{
				std::getline(fin, sLine);
				std::tistringstream sin_stream(sLine);

				std::tstring sName;
				sin_stream >> sName;

				if (mHierarchy.isCut(sName))
					continue;

				// skip되어야 할 관절들은 어떻게??
				if(mHierarchy.isSkipped(sName))
					continue;

				const CJoint* pJoint = 0;

				for (size_t i = 0; i < mpBody->getJointSize(); ++i)
				{
					if(mpBody->findByIndex(i)->getName() == sName)
					{
						pJoint = mpBody->findByIndex(i);
						break;
					}
				}

#ifdef _SKELETON
				assert(pJoint);
#endif
				if (0 == pJoint)
					return false;

				assert(pJoint->mDOF.mAmcOrderCount <= 7);
				for (unsigned int i = 0; i < pJoint->mDOF.mAmcOrderCount; ++i)
				{
					double dValue = 0.0;
					sin_stream >> dValue;

					if (!sin_stream)
						assert(false);

					if (pJoint->mDOF.mAmcOrder[i] == _T("RX") ||
						pJoint->mDOF.mAmcOrder[i] == _T("RY") ||
						pJoint->mDOF.mAmcOrder[i] == _T("RZ"))
					{
						if (mDegree)
							dValue = dValue * PI_D / 180.0;

						motionSeq[pJoint->getIndex()].mValues[i] = dValue;
					}
					else
					{
						motionSeq[pJoint->getIndex()].mValues[i] = dValue;
					}
				}
			}
		}

		return true;
	}



	bool CMotionLoader::loadAcclaimMotionFile(const std::wstring& _rFilePath)
	{
		if(mpBody == NULL)
		{
			assert("allocation skeleton pointer first");
			return false;
		}
		
		if(mpBody->emptyJoint())
		{
			assert(_T("Empty joints before loading motion files\n"));
			return false;
		}

		std::tifstream fin;
		fin.open(_rFilePath.c_str());

		if (!fin.is_open())
		{
			assert(_T("Failed to open amc file\n"));
			return false;
		}

		unsigned int nKeyframeCount = 0;

		TCHAR ch;
		std::tstring sBuffer;

		bool bResult = true;

		while (fin && bResult)
		{
			ch = fin.peek();
			if (!fin || fin.eof())
				break;
			else if (ch <= 0)
				break;

			if (_T('#') == ch)
			{
				std::getline(fin, sBuffer);
				continue;
			}
			else if (_T(':') == ch)
			{
				std::getline(fin, sBuffer);
				continue;
			}
			else if (isdigit(ch))
			{
				unsigned int new_keyframe = 0xffffffff;
				fin >> new_keyframe;
				assert(new_keyframe == nKeyframeCount + 1);

				nKeyframeCount	= new_keyframe;

				std::vector< CAMCkeyframe > motionSeq;	// motionSequence

				motionSeq.resize(mpBody->getJointSize());

				for(size_t it_ = 0 ; it_ < motionSeq.size() ; it_ ++)
					motionSeq[it_].mKeyIndex	= nKeyframeCount;

				if (!parseAmcKeyframe(fin, motionSeq))
				{
					assert(_T("Failed to parse amc keyframe\n"));
					bResult = false;
					break;
				}

				_insertAmcKeyframe(_rFilePath, nKeyframeCount, motionSeq);
			}
		}

		fin.clear();
		fin.close();
		
		mpBody->smoothingSequence();

		return true;
	}


	bool CMotionLoader::_insertAmcKeyframe(const std::tstring& sAmcFile,
		unsigned int nKeyframeIndex, std::vector< CAMCkeyframe >& _rMotionSequences)
	{
		if(_rMotionSequences.empty())
			return false;
		
		mpBody->insertKeyframe(sAmcFile, nKeyframeIndex, _rMotionSequences);

		return true;
	}



	// Connect By Hierarchy ------------------------------------------------------------
	//	그냥 연결하는 것이 아니라
	//	find를 하여 어떤 게 정말로 연결되어 있는 것이고
	//	어떤 것이 바로 연결을 하는 것이 아니라 translate 되어야 하는지를 알려준다.
	//	그러나 각각의 위치를 translate하는 과정이 필요한데. 어떻게 연결해야 하는지?
	bool CMotionLoader::_connectByHierarchy(CHierarchy* _pHierarchy)
	{
		for (auto it = _pHierarchy->begin(); it != _pHierarchy->end(); ++it)
		{
			if(!_pHierarchy->vaildData(it->first))
				continue;
			
			std::tstring sParent =	_pHierarchy->find(it->first)->first;
			//std::tstring sParent = it->first;
			CJoint* pParentJoint = mpBody->findByName(sParent);

			if(pParentJoint == NULL)
			{
				assert(_T("Failed to find hierarchy parent joint\n"));
				return false;
			}

			std::wstring childName;
			TStringArray& children = it->second;
			
			for (size_t i = 0; i < children.size(); ++i)
			{
				CJoint* pChildJoint = NULL;
				auto descent = _pHierarchy->find(it->second[i]);
				
				childName = descent->first;
				
				pChildJoint	= mpBody->findByName(childName);
				//(pChildJoint == NULL 일 수도 있음

				pChildJoint->setParent(pParentJoint);
				pParentJoint->addChild(pChildJoint);
			}
		}

		return true;
	}

	void	CMotionLoader::_setJointType(const CHierarchy& _rHierarchy)
	{
		for(size_t iter_ = 0 ; iter_ < mpBody->getJointSize() ; iter_ ++)
		{
			CJoint* joint	= mpBody->getIndex(iter_);

			if(_rHierarchy.isEndNode(joint->getName()))
				joint->mDynamics.mType	= FIXED_JOINT;

			else if(_rHierarchy.vaildData(joint->getName()))
				joint->mDynamics.mType	= REVOLUTE_JOINT;

		}
	}
}