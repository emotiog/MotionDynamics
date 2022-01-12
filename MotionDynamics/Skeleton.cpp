#include "stdafx.h"
#include "Skeleton.h"


namespace NAnimation
{
	CSkeleton::CSkeleton(void) :
		_mBodyPosition(0, 0, 0) ,
		_mBodyDirection(0, 0, 0, 1),
		mBodyMass(0),
		mMotionSequence(),
		mScale(10., 10., 10.)
	{
		_mName.clear();
		_mJoint.clear();

		m_ePushedDir	= PUSHED_ROOT ;
	}


	CSkeleton::~CSkeleton(void)
	{
	}


	CSkeleton::CSkeleton(const CSkeleton& _rSrc)
	{
		*this	= _rSrc;
	}


	CSkeleton& CSkeleton::operator=(const CSkeleton& _rSrc)
	{
		_mName		= _rSrc._mName;
		mBodyMass	= _rSrc.mBodyMass;

		_mBodyPosition	= _rSrc._mBodyPosition;
		_mBodyDirection	= _rSrc._mBodyDirection;

		_mJoint.resize(_rSrc._mJoint.size());

		m_ePushedDir	= _rSrc.m_ePushedDir;

		for(unsigned int it_ = 0 ; it_ < _rSrc._mJoint.size() ; it_ ++)
		{
			this->_mJoint[it_]	= _rSrc._mJoint[it_];
		}

		mScale	=	_rSrc.mScale;

		return (*this);
	}

	void CSkeleton::attachJoint(CJoint* _pJoint)
	{
		_mJoint.push_back(*_pJoint);
	}


	CJoint* CSkeleton::findByIndex(unsigned int _idx)
	{
		for(unsigned int it = 0 ; it < _mJoint.size() ; it ++)
		{
			if(_mJoint[it].getIndex() == _idx)
				return &_mJoint[it];
		}
		assert(_T("No matching index by given index"));
		return NULL;
	}


	CJoint* CSkeleton::getIndex(unsigned int _idx)
	{
		return &(_mJoint[_idx]);
	}


	unsigned int CSkeleton::getJointSize(void) const
	{
		return _mJoint.size();
	}


	void CSkeleton::resetIndex(void)
	{
		for(unsigned int it_ = 0 ; it_ < _mJoint.size() ; it_ ++)
		{
			_mJoint[it_].setIndex(it_);
		}
	}



	bool CSkeleton::connectByHierarchy(const std::map<std::tstring, TStringArray>& hierarchy)
	{
		for (auto it = hierarchy.begin(); it != hierarchy.end(); ++it)
		{
			std::tstring sParent = it->first;
			CJoint* pParentJoint = findByName(sParent);

			if(pParentJoint == NULL)
			{
				assert(_T("Failed to find hierarchy parent joint\n"));
				return false;
			}

			const TStringArray& children = it->second;
			for (size_t i = 0; i < children.size(); ++i)
			{
				CJoint* pChildJoint = findByName(children[i]);
				if (pChildJoint == NULL)
				{
					assert(_T("Failed to find hierarchy child joint\n"));
					return false;
				}

				pChildJoint->setParent(pParentJoint);
				pParentJoint->addChild(pChildJoint);
			}
		}

		return true;
	}


	void CSkeleton::reset(void)
	{
		_mName	= L"";
		_mJoint.clear();

		mBodyMass	= 1;
		mMotionSequence.reset();

		_mBodyPosition.zero();
		_mBodyDirection.x	= 0.f;
		_mBodyDirection.y	= 0.f;
		_mBodyDirection.z	= 0.f;
		_mBodyDirection.w	= 1.f;
	}


	bool CSkeleton::isParent(unsigned int _index)
	{
		if(_index > _mJoint.size())
		{
			assert("given index is over than limit!");
			return false;
		}

		if(_mJoint[_index].getParent() != NULL)
			return true;
		else
			return false;
	}


	bool CSkeleton::emptyJoint(void)
	{
		return _mJoint.empty();
	}


	void CSkeleton::resize(size_t size)
	{
		_mJoint.resize(size);
	}


	void CSkeleton::operator=(const std::vector<CJoint>& _rSrc)
	{
		assert(_rSrc.size() != _mJoint.size());

		for(unsigned int it_ = 0 ; it_ < _mJoint.size() ; it_ ++)
		{
			_mJoint[it_] = _rSrc[it_];
		}
	}


	const Vector3d& CSkeleton::getBodyPosition(void) const
	{
		return _mBodyPosition;
	}


	const Quaterniond& CSkeleton::getBodyDirection(void) const
	{
		return _mBodyDirection;
	}

	void	CSkeleton::setBodyPosition(float _x, float _y, float _z)
	{
		_mBodyPosition.x	= _x;
		_mBodyPosition.y	= _y;
		_mBodyPosition.z	= _z;
	}


	void	CSkeleton::setBodyPosition(const Vector3d& _rPos)
	{
		_mBodyPosition	= _rPos;
	}


	void	CSkeleton::erase(unsigned int _ith)
	{
		_mJoint.erase(_mJoint.begin() + _ith);
	}


	CJoint* CSkeleton::findByName(const std::tstring& _rStr)
	{
		for(unsigned int it_ = 0 ; it_ < _mJoint.size() ; it_ ++)
		{
			if( _mJoint[it_].getName() == _rStr)
				return &_mJoint[it_];
		}

		return NULL;
	}
	

	void CSkeleton::insertKeyframe(const std::tstring& sAmcFile,
		unsigned int nKeyframeIndex, std::vector< CAMCkeyframe >& _rMotionSequences)
	{
		for(size_t it_ = 0 ; it_ < _mJoint.size() ; it_ ++)
		{
			_mJoint[it_].insertAmcKeyframe(sAmcFile, nKeyframeIndex, _rMotionSequences[it_]);
		}
	}

	void CSkeleton::smoothingSequence(void)
	{
		// Start smoothing
		for (size_t i = 0; i < _mJoint.size(); ++i)
		{
			_mJoint[i].makeAnimationSequences(120.0f);
			_mJoint[i].makeSmoothSequences();
		}
	}


	void CSkeleton::animateSkeleton(float fElapseTime)
	{
		mMotionSequence._curAnimationTime	+= fElapseTime;

		unsigned int rootIndex = findByName(L"root")->getIndex();

		if(mMotionSequence._curAnimationTime > mMotionSequence._maxAnimationTime)
			mMotionSequence._curAnimationTime = mMotionSequence._minAnimationTime;

		for(unsigned int it_ = 0 ; it_ < _mJoint.size() ; it_ ++)
		{
			if(rootIndex == it_)
				_mJoint[it_].animateRoot(mMotionSequence._curAnimationTime, _mBodyPosition);
			else
				_mJoint[it_].animate(mMotionSequence._curAnimationTime);
		}
	}


	void CSkeleton::_updateDynamicsMaterials_Rev(CJoint* _pEndEf, CJoint* _pRoot)
	{
		CTransformMatrix	worldTransformMat;
		CTransformMatrix	_localMat;
		
		CTransformMatrix	worldMat;
		CTransformMatrix	localMat;

		int partSize	= _pEndEf->mHierarchyByCenter.size();
		std::vector<CTransformMatrix>	dT(partSize);
	
		_pEndEf->standFoot(&worldMat, &localMat);

		assert( partSize > 2 );
		for(int it_ = partSize -2 ; it_ > 0  ; it_ --)
		{
			CJoint*	curJoint	= NULL;
			CJoint* nextJoint	= NULL;
			curJoint	= _pEndEf->mHierarchyByCenter[it_];
			nextJoint	= _pEndEf->mHierarchyByCenter[it_ -1];

			// 여기 위치를 수정
			assert(curJoint->mDynamics.mType != FIXED_JOINT);
			curJoint->updateMatrixByDynamics(&worldMat, &localMat, nextJoint, mScale);

			CTransformMatrix Q	= curJoint->getTransformMatrixQ_quat();
			CTransformMatrix dQ	= curJoint->getTransformMatrixDiffQ_quat();

			curJoint->mDynamics.mTwistAxis	= curJoint->mDynamics.mLocalDir.normaliseVec();
			curJoint->mDynamics.mWorldPos	= worldTransformMat.getWorldPosition();

			for(int j_ = 0 ; j_ < partSize ; ++ j_)
			{
				unsigned int j_th	= _mJoint[j_].getIndex();
				// dz의 의미는 코드에서 joint 가 twist할 때의 벡터를 나타내는데,
				// 방향 벡터를 중심으로 얼마나 회전하는 가의 속도를 대입하는 것으로 이해했다.
				// 그렇다면 회전 축인 mDir 을 기준으로, 회전을 하게 된다면.. q 만큼 
				// 회전하게 될 것이다. 그 값을 대입하기로 했다.
				Quaternion<double> qzQ = NAnimation::MakeQuaternionFromRotationMatrix(dT[j_th]);

				float fTheta = 0.f;
				MakeAxisAndAngleFromQuaterniond(qzQ, &curJoint->mDynamics.dWorldAxis[j_th], fTheta);
				//_mJoint[it_].mDynamics.dz[j_]	= dT[j_].getRotation(_mJoint[it_].mDynamics.mAxis);
				curJoint->mDynamics.dWorldPos[j_th]	= dT[j_th].getWorldPosition();
			}

			// dT[ k_ ] = dT / dq[k번째] 나누는 과정
			// k_는 dt[15] 이므로 해당 joint의 index에 맞게
			for(int k_ = 0 ; k_ < partSize ; ++ k_)
			{	
				unsigned int k_th	= _mJoint[k_].getIndex();
				if( k_ > it_ )
					dT[k_th].zero();	// 관계 없는 것들
				else if( k_ == it_ )
					dT[k_th] = worldTransformMat * dQ;
				else
					dT[k_th] = dT[k_th] * Q;	// 나보다 root 혹은 thorax에 가까운 것들
			}

			worldTransformMat *= Q;


			curJoint->mDynamics.R			= worldTransformMat.getRotation();
			curJoint->mDynamics.mWorldCOM	= curJoint->mDynamics.mWorldPos +
				curJoint->mDynamics.R * curJoint->getCoM();

			for(int j_ = 0 ; j_ < partSize ; j_ ++)
			{
				unsigned int j_th	= _mJoint[j_].getIndex();
				curJoint->mDynamics.dR[j_th]	= dT[j_th].getRotation();
				curJoint->mDynamics.dWorldCoM[j_th]	= curJoint->mDynamics.dWorldPos[j_th] +
					curJoint->mDynamics.dR[j_th] * curJoint->getCoM();
			}

			for(int k_ = 0 ; k_ < partSize ; ++ k_)
			{
				unsigned int k_th	= _mJoint[k_].getIndex();
				dT[k_th] *= _localMat; 
			}
		}
	}


	void CSkeleton::_updateDynamicsMaterials(CJoint* _pEndEf, CJoint* _pRoot)
	{
		CTransformMatrix	worldTransformMat;
		CTransformMatrix	_localMat;
		
		unsigned int partSize	= _pEndEf->mHierarchyByCenter.size();

		std::vector<CTransformMatrix>	dT(partSize);

		CJoint*	pRoot	= findByName(L"root");

		for(unsigned int it_ = 0 ; it_ < partSize ; it_ ++)
		{
			CJoint*	curJoint = NULL;
			curJoint	= _pEndEf->mHierarchyByCenter[it_];

			// Dyanmics 값을 대입
			Quaterniond qDynamicRot = Quaternion<double>(0., 0., 0., 1.);	
			

			qDynamicRot = MakeQuaterniondFromAxisAndAngle(
				curJoint->mDynamics.mTwistAxis, curJoint->mDynamics.q);

			
			curJoint->animateWithDynamicQ(
				&worldTransformMat,
				&_localMat,
				&qDynamicRot,
				mMotionSequence._curAnimationTime,
				mScale,
				_mBodyPosition);

			CTransformMatrix Q	= curJoint->getTransformMatrixQ_quat();
			CTransformMatrix dQ	= curJoint->getTransformMatrixDiffQ_quat();

			curJoint->mDynamics.mTwistAxis	= curJoint->mDynamics.mLocalDir.normaliseVec();
			curJoint->mDynamics.mWorldPos	= worldTransformMat.getWorldPosition();

			for(unsigned int j_ = 0 ; j_ < partSize ; ++ j_)
			{
				unsigned int j_th	= _mJoint[j_].getIndex();
				// dz의 의미는 코드에서 joint 가 twist할 때의 벡터를 나타내는데,
				// 방향 벡터를 중심으로 얼마나 회전하는 가의 속도를 대입하는 것으로 이해했다.
				// 그렇다면 회전 축인 mDir 을 기준으로, 회전을 하게 된다면.. q 만큼 
				// 회전하게 될 것이다. 그 값을 대입하기로 했다.
				Quaternion<double> qzQ = NAnimation::MakeQuaternionFromRotationMatrix(dT[j_th]);

				float fTheta = 0.f;
				MakeAxisAndAngleFromQuaterniond(qzQ, &curJoint->mDynamics.dWorldAxis[j_th], fTheta);
				//_mJoint[it_].mDynamics.dz[j_]	= dT[j_].getRotation(_mJoint[it_].mDynamics.mAxis);
				curJoint->mDynamics.dWorldPos[j_th]	= dT[j_th].getWorldPosition();
			}

			// dT[ k_ ] = dT / dq[k번째] 나누는 과정
			// k_는 dt[15] 이므로 해당 joint의 index에 맞게
			for(unsigned int k_ = 0 ; k_ < partSize ; ++ k_)
			{	
				unsigned int k_th	= _mJoint[k_].getIndex();
				if( k_ > it_ )
					dT[k_th].zero();	// 관계 없는 것들
				else if( k_ == it_ )
					dT[k_th] = worldTransformMat * dQ;
				else
					dT[k_th] = dT[k_th] * Q;	// 나보다 root 혹은 thorax에 가까운 것들
			}

			worldTransformMat *= Q;


			curJoint->mDynamics.R			= worldTransformMat.getRotation();
			curJoint->mDynamics.mWorldCOM	= curJoint->mDynamics.mWorldPos +
				curJoint->mDynamics.R * curJoint->getCoM();

			for(unsigned int j_ = 0 ; j_ < partSize ; j_ ++)
			{
				unsigned int j_th	= _mJoint[j_].getIndex();
				curJoint->mDynamics.dR[j_th]	= dT[j_th].getRotation();
				curJoint->mDynamics.dWorldCoM[j_th]	= curJoint->mDynamics.dWorldPos[j_th] +
					curJoint->mDynamics.dR[j_th] * curJoint->getCoM();
			}

			for(unsigned int k_ = 0 ; k_ < partSize ; ++ k_)
			{
				unsigned int k_th	= _mJoint[k_].getIndex();
				dT[k_th] *= _localMat; 
			}
		}
	}


	void CSkeleton::adaptDynamicsForce(void)
	{
		CJoint* pRoot	= findByName(L"root");
		
		CJoint*	pLfoot	= findByName(L"ltibia");
		CJoint* pRfoot	= findByName(L"rtibia");

		CJoint*	pLhip	= findByName(L"lhipjoint");
		CJoint*	pRhip	= findByName(L"rhipjoint");

		CJoint* pThorax	= findByName(L"thorax");

		CJoint*	pUpNeck	= findByName(L"upperneck");

		CJoint*	pLwrist	= findByName(L"lradius");
		CJoint* pRwrist	= findByName(L"rradius");


#ifdef _SKELETON
		switch(m_ePushedDir)
		{
		case PUSHED_LEFT:
			_updateDynamicsMaterials_Rev(pLfoot, pUpNeck);
			_updateDynamicsMaterials( pRfoot, pRoot);
			break;

		case PUSHED_RIGHT:
			_updateDynamicsMaterials_Rev(pRfoot, pUpNeck);
			_updateDynamicsMaterials( pLfoot, pRoot);
			break;
			
		case PUSHED_ROOT:
			_updateDynamicsMaterials( pRfoot, pRoot);
			_updateDynamicsMaterials( pLfoot, pRoot);
			_updateDynamicsMaterials( pUpNeck, pRoot);
			break;
		}
		

		_updateDynamicsMaterials( pLwrist, pThorax);
		_updateDynamicsMaterials( pRwrist, pThorax);
#else
		animateSkeletonStatic15();
#endif

	}

	
	// ---- Animate Skeleton Static 15 --------------------------------------------------------
	//	worldTransformMat는 1개가 되어서는 안된다.
	//	각각의 노드들이 트리처럼 분산되어 있기 때문에 각각의 world transformMatrix는 따로
	//	존재하기 때문
	void CSkeleton::animateSkeletonStatic15(void)
	{
#ifdef _SKELETON
		CJoint* pRoot	= findByName(L"root");
		
		CJoint*	pLfoot	= findByName(L"ltibia");
		CJoint* pRfoot	= findByName(L"rtibia");

		CJoint*	pLhip	= findByName(L"lhipjoint");
		CJoint*	pRhip	= findByName(L"rhipjoint");

		CJoint* pThorax	= findByName(L"thorax");

		CJoint*	pUpNeck	= findByName(L"upperneck");

		CJoint*	pLwrist	= findByName(L"lradius");
		CJoint* pRwrist	= findByName(L"rradius");
		_updateDynamicsMaterials( pRfoot, pRoot);
		_updateDynamicsMaterials( pLfoot, pRoot);
		
		_updateDynamicsMaterials( pRoot, pThorax);
		_updateDynamicsMaterials( pUpNeck, pThorax);
		_updateDynamicsMaterials( pLwrist, pThorax);
		_updateDynamicsMaterials( pRwrist, pThorax);

#else
		static	unsigned int size	= _mJoint.size();
		unsigned int rootIndex = findByName(L"root")->getIndex();

		CTransformMatrix	worldTransformMat;
		CTransformMatrix _localMat;


		std::vector<CTransformMatrix>	dT(size);

		for(unsigned int it_ = 0 ; it_ < size ; it_ ++)
		{
			CJoint*	curJoint	= &_mJoint[it_];
			// Dyanmics 값을 대입
			Quaterniond qDynamicRot = Quaternion<double>(0., 0., 0., 1.);	
			
			qDynamicRot = MakeQuaterniondFromAxisAndAngle(
				_mJoint[it_].mDynamics.mTwistAxis, _mJoint[it_].mDynamics.q);
			
			_mJoint[it_].animateWithDynamicQ(
				&worldTransformMat,
				&_localMat,
				&qDynamicRot,
				mMotionSequence._curAnimationTime,
				mScale, _mBodyPosition);


			CTransformMatrix Q	= _mJoint[it_].getTransformMatrixQ_quat();
			CTransformMatrix dQ	= _mJoint[it_].getTransformMatrixDiffQ_quat();

			_mJoint[it_].mDynamics.mTwistAxis	= _mJoint[it_].mDynamics.mLocalDir.normaliseVec();
			_mJoint[it_].mDynamics.mWorldPos	= worldTransformMat.getWorldPosition();

			for(unsigned int j_ = 0 ; j_ < size ; ++ j_)
			{
				// dz의 의미는 코드에서 joint 가 twist할 때의 벡터를 나타내는데,
				// 방향 벡터를 중심으로 얼마나 회전하는 가의 속도를 대입하는 것으로 이해했다.
				// 그렇다면 회전 축인 mDir 을 기준으로, 회전을 하게 된다면.. q 만큼 
				// 회전하게 될 것이다. 그 값을 대입하기로 했다.
				Quaternion<double> qzQ = NAnimation::MakeQuaternionFromRotationMatrix(dT[j_]);

				float fTheta = 0.f;
				MakeAxisAndAngleFromQuaterniond(qzQ, &_mJoint[it_].mDynamics.dWorldAxis[j_], fTheta);
				//_mJoint[it_].mDynamics.dz[j_]	= dT[j_].getRotation(_mJoint[it_].mDynamics.mAxis);
				_mJoint[it_].mDynamics.dWorldPos[j_]	= dT[j_].getWorldPosition();
			}

			// dT[ k_ ] = dT / dq[k번째] 나누는 과정
			for(unsigned int k_ = 0 ; k_ < size ; ++ k_)
			{	
				if( k_ > it_ )
					dT[k_].zero();
				else if( k_ == it_ )
					dT[k_] = worldTransformMat * dQ;
				else
					dT[k_] = dT[k_] * Q;
			}

			worldTransformMat *= Q;


			_mJoint[it_].mDynamics.R			= worldTransformMat.getRotation();
			_mJoint[it_].mDynamics.mWorldCOM	= _mJoint[it_].mDynamics.mWorldPos +
				_mJoint[it_].mDynamics.R * _mJoint[it_].getCoM();

			for(unsigned int j_ = 0 ; j_ < size ; j_ ++)
			{
				_mJoint[it_].mDynamics.dR[j_]	= dT[j_].getRotation();
				_mJoint[it_].mDynamics.dWorldCoM[j_]	= _mJoint[it_].mDynamics.dWorldPos[j_] +
					_mJoint[it_].mDynamics.dR[j_] * _mJoint[it_].getCoM();
			}

			for(unsigned int k_ = 0 ; k_ < size ; ++ k_)
				dT[k_] *= _localMat;
		}
#endif
	}


	void CSkeleton::renderSkeleton(void)
	{
		for (size_t i = 0; i < _mJoint.size(); ++i)
		{
			_mJoint[i].renderBone();
		}
	}


	void CSkeleton::removeJoint(unsigned int _ith)
	{
		_mJoint.erase(_mJoint.begin() + _ith);
	}


	void	CSkeleton::centeralHierarchy(void)
	{
		CJoint* ptrCenterJoint	= NULL;
		CJoint* ptrCenterChild	= NULL;

		// 머리 
		ptrCenterJoint	= findByName(L"upperneck");
		ptrCenterJoint->mHierarchyByCenter.push_back(ptrCenterJoint);


		// 가슴
		ptrCenterChild	= findByName(L"thorax");
		
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// 머리 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;
		CJoint*	ptrCenterThorax	= ptrCenterChild;


		// root
		ptrCenterChild	= findByName(L"root");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// 가슴 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;
		CJoint*	ptrCenterRoot	= ptrCenterChild;


		// 왼쪽 엉덩이
		ptrCenterChild	= findByName(L"lhipjoint");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// root 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;	


		// 왼쪽 무릎
		ptrCenterChild	= findByName(L"lfemur");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// 엉덩이 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;	


		// 왼쪽 발 끝(사실 복숭아)
		ptrCenterChild	= findByName(L"ltibia");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter; // root 값 받기
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		


		// 오른쪽 엉덩이
		ptrCenterChild	= findByName(L"rhipjoint");
		ptrCenterChild->mHierarchyByCenter = ptrCenterRoot->mHierarchyByCenter; // root 값 받기
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;	


		// 오른쪽 무릎
		ptrCenterChild	= findByName(L"rfemur");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter; // root 값 받기
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

		ptrCenterJoint	= ptrCenterChild;	


		// 오른쪽 발 끝
		ptrCenterChild	= findByName(L"rtibia");
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter; // root 값 받기
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);

	

		// 왼쪽 어깨
		ptrCenterChild	= findByName(L"lclavicle");
		ptrCenterChild->mHierarchyByCenter.push_back(findByName(L"root"));			// root 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);				// thorax 추가		

		ptrCenterJoint	= ptrCenterChild;
		

		// 왼쪽 팔
		ptrCenterChild	= ptrCenterJoint->getChild();
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// 부모 값 복사
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);				// 바로 윗 부모 넣기
		
		ptrCenterJoint	= ptrCenterChild;
		
		// 왼쪽 손목
		ptrCenterChild	= ptrCenterJoint->getChild();
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;		// 부모 값 복사
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);				// 바로 윗 부모 넣기


		// 오른쪽 어깨
		ptrCenterChild	= findByName(L"rclavicle");
		ptrCenterChild->mHierarchyByCenter.push_back(findByName(L"root"));			// root 추가
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);					// 바로 윗 부모 넣기
				
		ptrCenterJoint	= ptrCenterChild;

		// 오른쪽 팔
		ptrCenterChild	= ptrCenterJoint->getChild();
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;	// 부모 값 복사
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);					// 바로 윗 부모 넣기

		ptrCenterJoint	= ptrCenterChild;

		// 오른쪽 손목
		ptrCenterChild	= ptrCenterJoint->getChild();
		ptrCenterChild->mHierarchyByCenter = ptrCenterJoint->mHierarchyByCenter;	// 부모 값 복사
		ptrCenterChild->mHierarchyByCenter.push_back(ptrCenterChild);					
	}

	bool	CSkeleton::isCollapse(void)
	{
		unsigned int size = _mJoint.size();
		static int lFootIdx	= findByName(L"ltibia")->getIndex();
		static int rFootIdx	= findByName(L"rtibia")->getIndex();

		for(unsigned int it_ = 0 ; it_ < size ; it_ ++)
		{
			// 발은 제외
			if(it_ == lFootIdx || it_ == rFootIdx)
				continue;

			else if(_mJoint[it_].isTouchedPlane())
				return true;
		}

		return false;
	}
}