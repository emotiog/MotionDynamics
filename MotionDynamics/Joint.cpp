#include "stdafx.h"
#include "Joint.h"
#include <assert.h>


#define max

namespace NAnimation
{
	CJoint::CDOF::CDOF(void)
	{
		mAmcOrderCount	= 0;

		for(int it_ = 0 ; it_ < 7 ; it_ ++)
			mAmcOrder[it_]	= 0.;
		
		dLimits[0][0] = dLimits[0][1] = 0.0;
		dLimits[1][0] = dLimits[1][1] = 0.0;
		dLimits[2][0] = dLimits[2][1] = 0.0;
		dLimits[3][0] = dLimits[3][1] = 0.0;
		dLimits[4][0] = dLimits[4][1] = 0.0;
		dLimits[5][0] = dLimits[5][1] = 0.0;
		dLimits[6][0] = dLimits[6][1] = 0.0;
	}


	CJoint::CDOF::~CDOF(void)
	{
	}
	
	CJoint::CDOF& CJoint::CDOF::operator=(const CDOF& _rSrc)
	{
		mAmcOrderCount	= _rSrc.mAmcOrderCount;

		for(int it_ = 0 ; it_ < 7 ; it_ ++)
			mAmcOrder[it_]	= _rSrc.mAmcOrder[it_];

		for(int icol = 0 ; icol < 7 ; icol ++)
		{
			for(int jrow = 0 ; jrow < 2 ; jrow ++)
			{
				dLimits[icol][jrow] = _rSrc.dLimits[icol][jrow];
			}
		}

		return (*this);
	}

	CJoint::CJoint(void)
		:eRotOrder(ROT_EULER_XYZ),
		mDOF(),
		_mpParent(NULL),
		_mLength(0),
		_mMass(1),
		_mLocalJointPos(0, 0, 0),
		_mLocalJointDir(0, 0, 0),
		_mLocalJointAxis(0, 0, 0),
		_mIndex(-1),
		cAnimation(),
		mCoM(0.f, 0.0f, 0.f)
	{
		for(int col = 0 ; col < 4 ; col ++)
			for(int row = 0 ; row < 4 ; row ++)
				if( col == row)
					mTransformMatrix.elements[col][row] = 1; 
				else
					mTransformMatrix.elements[col][row] = 0;

		_mPreRotation.identity();
		_mPostRotation.identity();
	}


	CJoint::CJoint(const CJoint& _rJnt)
	{
		*this = _rJnt;
	}


	CJoint::~CJoint(void)
	{
	}


	CJoint::CDynamics::CDynamics(void) :
	mLocalDir(0.f, 0.f, 1.f), dWorldCoM(15), dR(15)
	{
		dWorldAxis.resize(15);
		dWorldPos.resize(15);

		mWorldCOM.x	= 0.f;
		mWorldCOM.y	= 0.f;
		mWorldCOM.z	= 0.f;

		mAxis	= 0;

		mType	= REVOLUTE_JOINT;
		radius	= 1;

		q	= 0;
		dq	= 0;
		ddq	= 0;
	}

	CJoint::CDynamics&	CJoint::CDynamics::operator=(const CDynamics& _rSrc)
	{
		mType	= _rSrc.mType;
		q		= _rSrc.q;
		dq		= _rSrc.dq;
		ddq		= _rSrc.ddq;

		R		= _rSrc.R;
		mTwistAxis		= _rSrc.mTwistAxis;
		mWorldPos		= _rSrc.mWorldPos;
		mWorldCOM		= _rSrc.mWorldCOM;

		dR		= _rSrc.dR;

		return (*this);
	}

	void	CJoint::CDynamics::arrangeQ(void)
	{
		if(q > PI_D)
			q = q - PI_D;
		else if(q < -PI_D)
			q = q + PI_D;
	}


	CJoint& CJoint::operator=(const CJoint& _rJnt)
	{
		eRotOrder	= _rJnt.eRotOrder;
		_mIndex		= _rJnt._mIndex;
		_mName		= _rJnt._mName;

		_mpParent	= _rJnt._mpParent;
		_mpChildren	= _rJnt._mpChildren;

		_mLength	= _rJnt._mLength;
		_mMass		= _rJnt._mMass;

		_mLocalJointPos	= _rJnt._mLocalJointPos;
		_mLocalJointDir	= _rJnt._mLocalJointDir;
		_mLocalJointAxis= _rJnt._mLocalJointAxis;

		mDOF		= _rJnt.mDOF;

		mLocalMat			= _rJnt.mLocalMat;
		mTransformMatrix	= _rJnt.mTransformMatrix;

		cAnimation		= _rJnt.cAnimation;

		_mPreRotation	= _rJnt._mPreRotation;
		_mPostRotation	= _rJnt._mPostRotation;

		mHierarchyByCenter.resize(_rJnt.mHierarchyByCenter.size());
		mHierarchyByCenter	= _rJnt.mHierarchyByCenter;


		mDynamics	= _rJnt.mDynamics;

		return (*this);
	}


	void CJoint::setParent(CJoint* _pPar)
	{
		_mpParent = _pPar;
	}


	CJoint* CJoint::getParent(void) const
	{
		return _mpParent;
	}


	void CJoint::setIndex(unsigned int _idx)
	{
		_mIndex	= _idx;
	}


	unsigned int CJoint::getIndex(void) const
	{
		return _mIndex;
	}


	const CTransformMatrix&	CJoint::getTransformMatrix(void)
	{
		return mTransformMatrix;
	}


	const CTransformMatrix	CJoint::TransformMatrix(void)
	{
		return CTransformMatrix::CTransformMatrix( mTransformMatrix );
	}
	

	// ---- TransformMatrixQ -----------------------------------------------------
	// TransformMatrix - 축에 따라서 단순하게 나타낸 부분
	//	응용은 축에 따라서 다르게 분포하였을 경우에 가능
	const CTransformMatrix	CJoint::TransformMatrixQ(void)
	{
		if(mDynamics.mType == PRISMATIC_JOINT)
		{
			if(mDynamics.mAxis	== 0)
				return CTransformMatrix(mDynamics.q, 0, 0, 0, 0, 0);
			else if(mDynamics.mAxis	== 1)
				return CTransformMatrix(0, mDynamics.q, 0, 0, 0, 0);
			else if(mDynamics.mAxis	== 2)
				return CTransformMatrix(0, 0, mDynamics.q, 0, 0, 0);
		}
		else if(mDynamics.mType	== REVOLUTE_JOINT)
		{
			if(mDynamics.mAxis	== 0)
				return CTransformMatrix(0, 0, 0, mDynamics.q, 0, 0);
			else if(mDynamics.mAxis == 1)
				return CTransformMatrix(0, 0, 0, 0, mDynamics.q, 0);
			else if(mDynamics.mAxis == 2)
				return CTransformMatrix(0, 0, 0, 0, 0, mDynamics.q);
		}
		
		return CTransformMatrix(0, 0, 0, 0, 0, 0);
	}


	const CTransformMatrix	CJoint::getTransformMatrixQ_quat(void)
	{
		if(mDynamics.mType == PRISMATIC_JOINT)
		{
			Quaternion<double> qRot(0.f, 0.f, 0.f, 0.f);
			// 이동하고자 하는 vector
			CVector<double, 3> pos_dir = mDynamics.mWorldPos.normaliseVec();
			return CTransformMatrix(pos_dir * mDynamics.q, qRot);
		}
		else if(mDynamics.mType == REVOLUTE_JOINT)
		{
			// 쿼터니안에 따라 대입 q는 각 
			Quaternion<double> qRot	= MakeQuaterniondFromAxisAndAngle(mDynamics.mLocalDir, mDynamics.q);

			CVector<double, 3> pos(0.f, 0.f, 0.f);

			return CTransformMatrix(pos, qRot);
		}

		return CTransformMatrix(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	}



	// ---- Transform Matrix Derivation of Q --------------------------------------
	//	축이 단일 축이냐에 따라서 다음과 같이 나누어지는 것 같은데
	//	여러개의 축일 경우에는 어떻게 하는 것일까요
	const CTransformMatrix	CJoint::TransformMatrixDiffQ(void)
	{
		if(mDynamics.mType	== PRISMATIC_JOINT)
		{
			if(mDynamics.mAxis == 0)
				return CTransformMatrixDiffX(mDynamics.q, 0, 0, 0, 0, 0);
			else if(mDynamics.mAxis == 1)
				return CTransformMatrixDiffY(0, mDynamics.q, 0, 0, 0, 0);
			else if(mDynamics.mAxis	== 2)
				return CTransformMatrixDiffZ(0, 0, mDynamics.q, 0, 0, 0);
		}
		else if(mDynamics.mType	== REVOLUTE_JOINT)
		{
			if(mDynamics.mAxis	== 0)
				return CTransformMatrixDiffPhi	(0, 0, 0, mDynamics.q, 0, 0);
			else if(mDynamics.mAxis == 1)
				return CTransformMatrixDiffTheta(0, 0, 0, 0, mDynamics.q, 0);
			else if(mDynamics.mAxis == 2)
				return CTransformMatrixDiffPsi	(0, 0, 0, 0, 0, mDynamics.q);
		}

		return CTransformMatrixDiffX(0, 0, 0, 0, 0, 0);
	}


	const CTransformMatrix	CJoint::getTransformMatrixDiffQ_quat(void)
	{
		if(mDynamics.mType	== PRISMATIC_JOINT)
		{
			mDynamics.mLocalDir.normalise();
			CVector<double, 3> zero(0.f, 0.f, 0.f);
			return CTransformMatrix(mDynamics.mLocalDir, zero);
		}
		else if(mDynamics.mType	== REVOLUTE_JOINT)
		{
			mDynamics.mLocalDir.normalise();

			return CTransformMatrixDiffQuaternion(mDynamics.mLocalDir, mDynamics.q);
		}

		return CTransformMatrix(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	}


	void CJoint::updatePositionToTransformMatrix(void)
	{// 업데이트 되는 것이
		mTransformMatrix.m14	= _mLocalJointPos.x;
		mTransformMatrix.m24	= _mLocalJointPos.y;
		mTransformMatrix.m34	= _mLocalJointPos.z;
	}


	void CJoint::updateDirectionToTransformMatrix(void)
	{
		// Degree일까? - 읽어올 때 Degree 라면 Radian 으로 바꾸어 놨음 (parseBoneData 참조)
		Quaterniond	qRot	= MakeQuaterniondFromEulerRadianAngles(_mLocalJointDir, eRotOrder);
		mTransformMatrix	= MakeRotationMatrix4x4dFromQuaternion(qRot);
	}



	void CJoint::addChild(CJoint* _pChild)
	{
		for(unsigned int it_ = 0 ; it_ < _mpChildren.size() ; it_ ++)
		{
			if(_mpChildren[it_] == _pChild)
			{
				assert("Same Child already exists.");
				return;
			}
		}

		_mpChildren.push_back(_pChild);
	}


	// 1개 이상일 때는 불러올 수 없음
	CJoint*		CJoint::getChild(unsigned int index) const
	{
		assert(index < _mpChildren.size());
		return _mpChildren[index];
	}


	std::wstring	CJoint::getName(void) const
	{
		return _mName;
	}


	void	CJoint::setName(std::wstring _rStr)
	{
		_mName	= _rStr;
	}


	void	CJoint::setLength(double _rlngth)
	{
		_mLength = _rlngth;
	}


	double	CJoint::getLength(void) const
	{
		return _mLength;
	}


	void	CJoint::setMass(double _mass)
	{
		_mMass	= _mass;
	}

	double	CJoint::getMass(void) const
	{
		return _mMass;
	}

	void	CJoint::setLocalJointPosition(const CVector<double, 3>& _rPos)
	{
		_mLocalJointPos	= _rPos;
		
		mPreLocalPos	= _mLocalJointPos;
		updatePositionToTransformMatrix();
	}


	void	CJoint::setLocalJointPosition(double _x, double _y, double _z)
	{
		_mLocalJointPos.x	= _x;
		_mLocalJointPos.y	= _y;
		_mLocalJointPos.z	= _z;
		
		mPreLocalPos	= _mLocalJointPos;
		updateDirectionToTransformMatrix();
	}


	Vector3d	CJoint::getLocalJointPosition(void) const
	{
		return _mLocalJointPos;
	}


	void	CJoint::setLocalJointAxis(const Vector3d& _rDir)
	{
		_mLocalJointAxis = _rDir;

		updatePositionToTransformMatrix();
	}


	void	CJoint::setLocalJointAxis(double _x, double _y, double _z)
	{
		_mLocalJointAxis.x	= _x;
		_mLocalJointAxis.y	= _y;
		_mLocalJointAxis.z	= _z;

		updatePositionToTransformMatrix();
	}


	void	CJoint::setLocalJointDirection(double _x, double _y, double _z)
	{
		_mLocalJointDir.x	= _x;
		_mLocalJointDir.y	= _y;
		_mLocalJointDir.z	= _z;
	}


	Vector3d	CJoint::getLocalJointDirection(void) const
	{
		return _mLocalJointDir;
	}

	CVector<double, 3>	CJoint::getLocalJointAxis(void) const
	{
		return _mLocalJointAxis;
	}


	CVector<double, 3>	CJoint::getCoM(void) const
	{
		return mCoM;
	}


	bool		CJoint::isName(void)
	{
		return _mName.empty();
	}


	void	CJoint::activeSequence(const std::tstring& _rSequenceName)
	{
		cAnimation.activeSequence(_rSequenceName);
	}


	double	CJoint::getAnimationStartTime(void) const
	{
		return cAnimation.getAnimationStartTime();
	}


	double	CJoint::getAnimationStopTime(void) const
	{
		return cAnimation.getAnimationStopTime();
	}


	bool CJoint::insertAmcKeyframe(const std::tstring& sAmcFile,
			unsigned int nKeyframeIndex, const CAMCkeyframe& keyframe)
	{
		std::vector<CAMCkeyframe>& keyframes = cAnimation._mMotionSequences[sAmcFile];
		
		if (static_cast<unsigned int>(keyframes.size() + 1) != nKeyframeIndex)
			return false;

		if (keyframes.empty())
			keyframes.reserve(256);

		keyframes.push_back(keyframe);

		return true;
	}


	void	CJoint::makeSmoothSequences(void)
	{
		cAnimation.makeSmoothSequences();
	}


	void	CJoint::makeAnimationSequences(double fFPS)
	{
		cAnimation.makeAnimationSequences(mDOF, eRotOrder, fFPS);
	}


	void CJoint::CJointMotion::makeAnimationSequences(const CDOF& _rDOF, 
		ENUM_ROTATION_TYPE& _rType, double fFPS)
	{
		// Start to generate animation sequences
		_mSequences.clear();

		for (auto it = _mMotionSequences.begin(); it != _mMotionSequences.end(); ++it)
		{
			CSequence& seq = _mSequences[it->first];
			seq.createFromAmcKeyframes(_rDOF.mAmcOrder, _rDOF.mAmcOrderCount,
				_rType, it->second, fFPS);
		}
	}


	std::vector<CJoint*>	CJoint::getReverseHierarchy(void)
	{
		std::vector<CJoint*>	reverseVec;

		unsigned int hSize	= mHierarchyByCenter.size();

		for(int it = hSize -1 ; it >= 0 ; it --)
		{
			reverseVec.push_back(mHierarchyByCenter[it]);
		}

		return reverseVec;
	}


	void CJoint::CJointMotion::makeSmoothSequences(void)
	{
		for (auto it = _mSequences.begin(); it != _mSequences.end(); ++it)
		{
			CSequence& seq = it->second;
			seq.smoothing();
		}
	}


	CSequence* CJoint::CJointMotion::getSequenceByName(const std::tstring& sSequenceName)
	{
		auto it = _mSequences.find(sSequenceName);

		if (it != _mSequences.end())
			return &it->second;
		else
			return 0;
	}


	void CJoint::CJointMotion::activeSequence(const std::tstring& _rSequenceName)
	{
		CSequence* sequence = getSequenceByName(_rSequenceName);

#ifdef _SKELETON
		assert(0 != sequence);
#endif
		_mActiveSequence	= sequence;
	}

	double CJoint::CJointMotion::getAnimationStartTime(void) const
	{
		if(_mActiveSequence)
			return _mActiveSequence->getAnimationStartTime();
		else
			return 0.f;
	}

	double CJoint::CJointMotion::getAnimationStopTime(void) const
	{
		if(_mActiveSequence)
			return _mActiveSequence->getAnimationStopTime();
		else
			return 0.f;
	}


	const Matrix4x4d& CJoint::getPreRotation(void) const
	{
		return _mPreRotation;
	}


	const Matrix4x4d& CJoint::getPostRotation(void) const
	{
		return _mPostRotation;
	}


	CTransformMatrix CJoint::animate(double fCurTime)
	{
		assert(cAnimation._mActiveSequence != NULL);

		assert(this != NULL);

		// 모션으로 위치와 각도를 불러오는 곳
		const Vector3d vPos		= getAnimatedPosition(fCurTime);
		const Quaterniond qRot	= getAnimatedRotation(fCurTime);

		// Make Animation Matrix
		const Matrix4x4d& preRotation	= getPreRotation();
		const Matrix4x4d& postRotation	= getPostRotation();

		Matrix4x4d mTransform = MakeRotationMatrix4x4dFromQuaternion(qRot);
		mTransform = preRotation * mTransform * postRotation;

		// 현재 관절 fix 위치 + 모션 값 + dynamics 값! + 모션과 Dynamics는 전혀 다른 것으로
		Vector3d vTargetPos = mTransform * getLocalJointPosition() + vPos;
		
		mCoM	= vTargetPos /2. ;
		if (false == isValidValue(vTargetPos.x) ||
			false == isValidValue(vTargetPos.y) ||
			false == isValidValue(vTargetPos.z))
		{
			assert(_T("Invalid values are in the Target Pos when animating\n"));
		}

		assert(_mpParent != NULL);

		mTransformMatrix	 = mTransform;
		mTransformMatrix.m14 = vTargetPos.x;
		mTransformMatrix.m24 = vTargetPos.y;
		mTransformMatrix.m34 = vTargetPos.z;
		

		CTransformMatrix	myTransformMat = mTransformMatrix;

		CJoint* pParent = this->getParent();
		assert(0 != pParent);
		mTransformMatrix = pParent->getTransformMatrix() * mTransformMatrix;


		return myTransformMat;
	}


	void	CJoint::animateWithDynamicQ(
				CTransformMatrix* _pWorldMat,
				CTransformMatrix*	_pLocalMat,
				Quaterniond* qDynamicRot,
				double fCurTime,
				const CVector<double, 3>& _rScale,
				const CVector<double, 3>& _worldPos,
				const Quaternion<double>& _worldDir)
	{
#ifdef _SKELETON
		assert(cAnimation._mActiveSequence != NULL);

		assert(this != NULL);

		// 모션으로 위치와 각도를 불러오는 곳
		const Vector3d vPos		= getAnimatedPosition(fCurTime);
		const Quaterniond qRot	= getAnimatedRotation(fCurTime);
#endif
		// Make Animation Matrix
		const Matrix4x4d& preRotation	= getPreRotation();
		const Matrix4x4d& postRotation	= getPostRotation();
		
#ifdef _SKELETON
		Quaterniond	finalQ		= qRot.Slerp(*qDynamicRot, 0.5);//qRot * qDynamicRot;
		finalQ.Normalize();

		Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(finalQ);
#else
		Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(*qDynamicRot);
#endif


		mTransform = preRotation * mTransform * postRotation;

		//Quaterniond newQ	= MakeQuaternionFromRotationMatrix(mTransform);
		//MakeAxisAndAngleFromQuaterniond(newQ, &mDynamics.mDir, mDynamics.q);

		CVector<double, 3> scaledPos	= getLocalJointPosition();
		scaledPos.x *= _rScale.x;
		scaledPos.y *= _rScale.y;
		scaledPos.z *= _rScale.z;

#ifdef _SKELETON
		// 현재 관절 fix 위치 + 모션 값 + dynamics 값! + 모션과 Dynamics는 전혀 다른 것으로
		Vector3d vTargetPos = mTransform * scaledPos + vPos;
#else
		Vector3d vTargetPos = mTransform * scaledPos;
#endif


		if (false == isValidValue(vTargetPos.x) ||
			false == isValidValue(vTargetPos.y) ||
			false == isValidValue(vTargetPos.z))
		{
			assert(_T("Invalid values are in the Target Pos when animating\n"));
		}

		if(_mpParent != NULL)
		{
			mCoM	= vTargetPos / 2.;

			mTransformMatrix	 = mTransform;
			mTransformMatrix.m14 = vTargetPos.x;
			mTransformMatrix.m24 = vTargetPos.y;
			mTransformMatrix.m34 = vTargetPos.z;


			// 회전 축 지정
			//	회전 축의 정의: Dynamics에 영향을 주는 회전 축을 지정
			//	v1 에서는 2D 평면에 힘을 가하므로 z축의 값을 회전 축으로 지정함
		#ifdef	VERSION_1
			mDynamics.mLocalDir = mPreLocalPos.crossProduct(vTargetPos);
			mDynamics.mLocalDir.normalise();
		#endif
			// local은 값 복사
			*_pLocalMat = mTransformMatrix;
			mLocalMat	= mTransformMatrix;

			mPreLocalPos	= vTargetPos;

			CJoint* pParent = this->getParent();
			assert(0 != pParent);
			mTransformMatrix = pParent->getTransformMatrix() * mTransformMatrix;

			// 갱신될 수 있으니 값을 복사
			*_pWorldMat	= mTransformMatrix;
		}
		else
		{
			mCoM	= vTargetPos;	// Root는 점이기 때문에 그 점이 곧 무게중심

			mPreLocalPos	= vTargetPos;

			const Vector3d& v = _worldPos;

			Vector3d vOrigin(0.0f, 0.0f, 1.0f);
			Vector3d vTarget(mTransform.m31, 0.0f, mTransform.m33);

			double fAngle = acos(vOrigin.dotProduct(vTarget));
			Vector3d vAxis = vOrigin.crossProduct(vTarget);
			vAxis.normalise();

			Quaterniond qCur = Quaterniond(
				vAxis.x * sin(fAngle * 0.5f),
				vAxis.y * sin(fAngle * 0.5f),
				vAxis.z * sin(fAngle * 0.5f),
				cos(fAngle * 0.5f));

			Quaterniond q = _worldDir;
			qCur.Inverse();
			q *= qCur;

			Matrix4x4d m = MakeRotationMatrix4x4dFromQuaternion(q);
			mTransform = m * mTransform;

			mTransformMatrix = mTransform;
			mTransformMatrix.m14 = vTargetPos.x + v.x;
			mTransformMatrix.m24 = vTargetPos.y + v.y;
			mTransformMatrix.m34 = vTargetPos.z + v.z;

			// mTransform - local 좌표
			// mTransformMatrix - world 좌표
			*_pWorldMat	= mTransformMatrix;
			*_pLocalMat	= mTransform;
			mLocalMat	= mTransform;
		}
	}



	CTransformMatrix	CJoint::animateRoot(double fCurTime, const Vector3d& _worldPos, 
		const Quaterniond& _worldDir)
	{
		assert(cAnimation._mActiveSequence != NULL);

		assert(this != NULL);

		const Vector3d vPos		= getAnimatedPosition(fCurTime);
		const Quaterniond qRot	= getAnimatedRotation(fCurTime);

		// Make Animation Matrix
		const Matrix4x4d& preRotation	= getPreRotation();
		const Matrix4x4d& postRotation	= getPostRotation();

		Matrix4x4d mTransform = MakeRotationMatrix4x4dFromQuaternion(qRot);
		mTransform = preRotation * mTransform * postRotation;

		Vector3d vTargetPos = mTransform * getLocalJointPosition() + vPos;
		
		mCoM	= vTargetPos / 2.;

		if (false == isValidValue(vTargetPos.x) ||
			false == isValidValue(vTargetPos.y) ||
			false == isValidValue(vTargetPos.z))
		{
			assert(_T("Invalid values are in the Target Pos when animating\n"));
		}

		const Vector3d& v = _worldPos;

		Vector3d vOrigin(0.0f, 0.0f, 1.0f);
		Vector3d vTarget(mTransform.m31, 0.0f, mTransform.m33);

		double fAngle = acos(vOrigin.dotProduct(vTarget));
		Vector3d vAxis = vOrigin.crossProduct(vTarget);
		vAxis.normalise();

		Quaterniond qCur = Quaterniond(
			vAxis.x * sin(fAngle * 0.5f),
			vAxis.y * sin(fAngle * 0.5f),
			vAxis.z * sin(fAngle * 0.5f),
			cos(fAngle * 0.5f));

		Quaterniond q = _worldDir;
		qCur.Inverse();
		q *= qCur;

		Matrix4x4d m = MakeRotationMatrix4x4dFromQuaternion(q);
		mTransform = m * mTransform;

		mTransformMatrix = mTransform;
		mTransformMatrix.m14 = vTargetPos.x + v.x;
		mTransformMatrix.m24 = vTargetPos.y + v.y;
		mTransformMatrix.m34 = vTargetPos.z + v.z;

		return mTransformMatrix;
	}



	// ------ Stand Foot ---------------------------------
	//	2013.11.29
	//	지탱이 되는 시작 발(왼쪽 또는 오른쪽, 또는 양발?)에만 한정하여 적용
	//
	void	CJoint::standFoot(CJoint* _pStart,
				CJoint*	_pChild,
				CTransformMatrix* _pWorldMat,
				CTransformMatrix*	_pLocalMat,
				double fCurTime,
				const CVector<double, 3>& _rScale,
				const Vector3d&		_vTransPos,
				const Quaterniond&	_vRotateDir)
	{	// foot -> root 까지
		// animate 에서 나오는 회전 량은 root에서 빠져 나온 local value임
		
		// Dyanmics 값을 대입
		Quaterniond qAccumRot = MakeQuaterniondFromAxisAndAngle(mDynamics.mTwistAxis, mDynamics.q);

		Quaternion<double> matQ	= MakeQuaternionFromRotationMatrix(mTransformMatrix);
		Quaternion<double> finQ	= matQ.Slerp(qAccumRot, 0.5);
		
		finQ.Normalize();
		Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(finQ);

		//CMatrix<double, 4, 4>	m;

		//CVector<double, 3>	scalePos;
		//CVector<double, 3>	vTargetPos;


		mTransform.m14	= mTransformMatrix.m14;
		mTransform.m24	= mTransformMatrix.m24;
		mTransform.m34	= mTransformMatrix.m34;

		mTransformMatrix	= mTransform;


		*_pLocalMat	= mTransform;
		mLocalMat	= mTransform;
		*_pWorldMat	= mTransformMatrix;

		//if(this != _pStart)
		//{
		//	mTransformMatrix.m14	= 0.;
		//	mTransformMatrix.m24	= 0.;
		//	mTransformMatrix.m34	= 0.;
		//	

		//	if(this->mDynamics.mType == FIXED_JOINT)
		//	{
		//		if(_pChild->getParent() == this)	// root
		//		{
		//			vTargetPos	= mTransform * _pChild->getLocalJointPosition();
		//			vTargetPos	*= -1;

		//			m	= _pChild->mTransformMatrix;
		//		}
		//		else
		//		{// 종단 노드들
		//			
		//		}
		//	}
		//	else
		//	{
		//		vTargetPos	= mTransform * getChild()->getLocalJointPosition();
		//		vTargetPos *= -1;

		//		m	= getChild()->mTransformMatrix;
		//	}

		//	mTransform.m14	= vTargetPos.x * _rScale.x;	// 역으로 올라가는 경우만 -1 해줌
		//	mTransform.m24	= vTargetPos.y * _rScale.y;
		//	mTransform.m34	= vTargetPos.z * _rScale.z;

		//	mTransformMatrix	= m * mTransform;
		//}
		//else
		//{
		//	//Vector3d vOrigin(0.0f, 0.0f, 1.0f);
		//	//Vector3d vTarget(mTransform.m31, 0.0f, mTransform.m33);

		//	//double fAngle = acos(vOrigin.dotProduct(vTarget));
		//	//Vector3d vAxis = vOrigin.crossProduct(vTarget);
		//	//vAxis.normalise();

		//	//Quaterniond qCur = Quaterniond(
		//	//	vAxis.x * sin(fAngle * 0.5f),
		//	//	vAxis.y * sin(fAngle * 0.5f),
		//	//	vAxis.z * sin(fAngle * 0.5f),
		//	//	cos(fAngle * 0.5f));

		//	//Quaterniond q = _vRotateDir;
		//	//qCur.Inverse();
		//	//q *= qCur;

		//	//m = MakeRotationMatrix4x4dFromQuaternion(q);
		//	//mTransform = m * mTransform;

		//	mTransform.m14	= mTransformMatrix.m14;
		//	mTransform.m24	= mTransformMatrix.m24;
		//	mTransform.m34	= mTransformMatrix.m34;

		//	mTransformMatrix	= mTransform;
		//}

		//*_pLocalMat	= mTransform;
		//mLocalMat	= mTransform;
		//*_pWorldMat	= mTransformMatrix;
	}


	void	CJoint::standFoot(CTransformMatrix* _pWorldMat,	CTransformMatrix*	_pLocalMat)
	{
		Quaterniond qAccumRot = MakeQuaterniondFromAxisAndAngle(mDynamics.mTwistAxis, mDynamics.q);

		Quaternion<double> matQ	= MakeQuaternionFromRotationMatrix(mTransformMatrix);
		Quaternion<double> finQ	= matQ.Slerp(qAccumRot, 0.5);
		
		finQ.Normalize();
		Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(finQ);

		mTransform.m14	= mTransformMatrix.m14;
		mTransform.m24	= mTransformMatrix.m24;
		mTransform.m34	= mTransformMatrix.m34;

		mTransformMatrix	= mTransform;


		*_pLocalMat	= mTransform;
		mLocalMat	= mTransform;
		*_pWorldMat	= mTransformMatrix;
	}


	void	CJoint::updateMatrixByDynamics(CTransformMatrix* _pWorldMat,	
		CTransformMatrix*	_pLocalMat, 
		const CJoint*	_pChild,
		const CVector<double, 3>& _rScale)
	{
		Quaterniond qAccumRot = MakeQuaterniondFromAxisAndAngle(mDynamics.mTwistAxis, mDynamics.q);

		Quaternion<double> matQ	= MakeQuaternionFromRotationMatrix(mTransformMatrix);
		Quaternion<double> finQ	= matQ.Slerp(qAccumRot, 0.5);
		
		finQ.Normalize();
		CMatrix<double, 4, 4>	mTransform	= MakeRotationMatrix4x4dFromQuaternion(finQ);
		CMatrix<double, 4, 4>	childMat;

		mTransformMatrix.m14	= 0.;
		mTransformMatrix.m24	= 0.;
		mTransformMatrix.m34	= 0.;

		CVector<double, 3>	vTargetPos	= mTransform * _pChild->getLocalJointPosition();
		vTargetPos *= -1;

		childMat	= _pChild->mTransformMatrix;

		mTransform.m14	= vTargetPos.x * _rScale.x;	// 역으로 올라가는 경우만 -1 해줌
		mTransform.m24	= vTargetPos.y * _rScale.y;
		mTransform.m34	= vTargetPos.z * _rScale.z;

		mTransformMatrix	= childMat * mTransform;

		*_pLocalMat	= mTransform;
		mLocalMat	= mTransform;
		*_pWorldMat	= mTransformMatrix;
	}

	//void	CJoint::computeForce(CJoint* _pStart,
	//			CTransformMatrix* _pWorldMat,
	//			CTransformMatrix*	_pLocalMat,
	//			Quaterniond* qRot,
	//			double fCurTime,
	//			const Vector3d&		_vTransPos,
	//			const Quaterniond&	_vRotateDir)
	//{

	//// 0. 현재 위치 - 기존 모션 + q 값
	//	// 모션으로 위치와 각도를 불러오는 곳
	//	const Vector3d vPos			= getAnimatedPosition(fCurTime);
	//	const Quaterniond qMotionRot= getAnimatedRotation(fCurTime);

	//	// Make Animation Matrix
	//	const Matrix4x4d& preRotation	= getPreRotation();
	//	const Matrix4x4d& postRotation	= getPostRotation();
	//	
	//	Quaternion<double>	quat	= qRot->Slerp(*qRot, 0.5);
	//	quat.Normalize();

	//	Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(quat);

	//	mTransform = preRotation * mTransform * postRotation;

	//	// 현재 관절 fix 위치 + 모션 값 + dynamics 값! + 모션과 Dynamics는 전혀 다른 것으로
	//	Vector3d vTargetPos = mTransform * getLocalJointPosition() + vPos;
	//	mCoM	= vTargetPos / 2.;

	//	if (false == isValidValue(vTargetPos.x) ||
	//		false == isValidValue(vTargetPos.y) ||
	//		false == isValidValue(vTargetPos.z))
	//	{
	//		assert(_T("Invalid values are in the Target Pos when animating\n"));
	//	}

	//	assert(_mpParent != NULL);

	//	mTransformMatrix	 = mTransform;
	//	mTransformMatrix.m14 = vTargetPos.x;
	//	mTransformMatrix.m24 = vTargetPos.y;
	//	mTransformMatrix.m34 = vTargetPos.z;

	//	// local은 값 복사
	//	*_pLocalMat = mTransformMatrix;
	//	mLocalMat	= mTransformMatrix;

	//	mPreLocalPos	= vTargetPos;

	//	CJoint* pParent = this->getParent();
	//	assert(0 != pParent);
	//	mTransformMatrix = pParent->getTransformMatrix() * mTransformMatrix;

	//	// 갱신될 수 있으니 값을 복사
	//	*_pWorldMat	= mTransformMatrix;
	//}




	//void	CJoint::animateRootWithDynamicQ(
	//										CTransformMatrix* _pWorldMat, 
	//										CTransformMatrix*	_pLocalMat,
	//										Quaterniond* qDynamicRot,
	//										double fCurTime,
	//										const CVector<double, 3>& _rScale,
	//										const Vector3d& _worldPos, 
	//										const Quaterniond& _worldDir)
	//{
	//	assert(cAnimation._mActiveSequence != NULL);

	//	assert(this != NULL);

	//	const Vector3d		vPos	= getAnimatedPosition(fCurTime);
	//	const Quaterniond	qRot	= getAnimatedRotation(fCurTime);

	//	// Make Animation Matrix
	//	const Matrix4x4d& preRotation	= getPreRotation();
	//	const Matrix4x4d& postRotation	= getPostRotation();

	//	Quaterniond	finalQ	= qRot.Slerp(*qDynamicRot, 0.5);//qRot * qDynamicRot;
	//	
	//	finalQ.Normalize();

	//	Matrix4x4d mTransform	= MakeRotationMatrix4x4dFromQuaternion(finalQ);
	//	mTransform = preRotation * mTransform * postRotation;


	//	CVector<double, 3> scaledPos	= getLocalJointPosition();
	//	scaledPos.x *= _rScale.x;
	//	scaledPos.y *= _rScale.y;
	//	scaledPos.z *= _rScale.z;


	//	// 현재 관절 fix 위치 + 모션 값 + dynamics 값! + 모션과 Dynamics는 전혀 다른 것으로
	//	Vector3d vTargetPos = mTransform * scaledPos + vPos;
	//	
	//	mCoM	= vTargetPos;	// Root는 점이기 때문에 그 점이 곧 무게중심

	//	if (false == isValidValue(vTargetPos.x) ||
	//		false == isValidValue(vTargetPos.y) ||
	//		false == isValidValue(vTargetPos.z))
	//	{
	//		assert(_T("Invalid values are in the Target Pos when animating\n"));
	//	}

	//	mPreLocalPos	= vTargetPos;

	//	const Vector3d& v = _worldPos;

	//	Vector3d vOrigin(0.0f, 0.0f, 1.0f);
	//	Vector3d vTarget(mTransform.m31, 0.0f, mTransform.m33);

	//	double fAngle = acos(vOrigin.dotProduct(vTarget));
	//	Vector3d vAxis = vOrigin.crossProduct(vTarget);
	//	vAxis.normalise();

	//	Quaterniond qCur = Quaterniond(
	//		vAxis.x * sin(fAngle * 0.5f),
	//		vAxis.y * sin(fAngle * 0.5f),
	//		vAxis.z * sin(fAngle * 0.5f),
	//		cos(fAngle * 0.5f));

	//	Quaterniond q = _worldDir;
	//	qCur.Inverse();
	//	q *= qCur;

	//	Matrix4x4d m = MakeRotationMatrix4x4dFromQuaternion(q);
	//	mTransform = m * mTransform;

	//	mTransformMatrix = mTransform;
	//	mTransformMatrix.m14 = vTargetPos.x + v.x;
	//	mTransformMatrix.m24 = vTargetPos.y + v.y;
	//	mTransformMatrix.m34 = vTargetPos.z + v.z;

	//	// mTransform - local 좌표
	//	// mTransformMatrix - world 좌표
	//	*_pWorldMat	= mTransformMatrix;
	//	*_pLocalMat	= mTransform;
	//	mLocalMat	= mTransform;
	//}


	Vector3d CJoint::getAnimatedPosition(double fCurTime) const
	{
		assert(0 != cAnimation._mActiveSequence);
		assert(0 != this);

		Vector3d vPos(0.0, 0.0, 0.0);
		if (cAnimation._mActiveSequence->isContainedPosKeys())
		{
			const CPosKey* pKeyFirst = 0;
			const CPosKey* pKeySecond = 0;
			cAnimation.lastPosKeyIndex_ = cAnimation._mActiveSequence->findInbetweenPosKeys(
				fCurTime, pKeyFirst, pKeySecond, cAnimation.lastPosKeyIndex_);
			assert(pKeyFirst);
			assert(pKeySecond);

			double t = 0.0f;
			if (abs(pKeySecond->fTime - pKeyFirst->fTime) > 0.00001f)
				t = (pKeySecond->fTime - fCurTime) / (pKeySecond->fTime - pKeyFirst->fTime);
			else
				t = 0.5f;

			if (isValidValue(t))
				t = min(max(t, 0.0f), 1.0f);
			else
				t = 0.5f;

			vPos = pKeyFirst->vPos + (pKeySecond->vPos - pKeyFirst->vPos) * t;
		}

		return vPos;
	}


	Quaterniond CJoint::getAnimatedRotation(double fCurTime) const
	{
		assert(0 != cAnimation._mActiveSequence);
		assert(0 != this);

		Quaterniond qRot;
		if (cAnimation._mActiveSequence->isContainedRotKeys())
		{
			const CRotKey* pKeyFirst = 0;
			const CRotKey* pKeySecond = 0;

			cAnimation.lastRotKeyIndex_ = cAnimation._mActiveSequence->findInbetweenRotKeys(
				fCurTime, pKeyFirst, pKeySecond, cAnimation.lastRotKeyIndex_);
			assert(pKeyFirst);
			assert(pKeySecond);

			double t = 0.0f;
			
			if (abs(pKeySecond->fTime - pKeyFirst->fTime) > 0.00001f)
				t = (pKeySecond->fTime - fCurTime) / (pKeySecond->fTime - pKeyFirst->fTime);
			else
				t= 0.5f;

			if (isValidValue(t))
				t = min(max(t, 0.0f), 1.0f);
			else
				t = 0.5f;

			qRot = pKeyFirst->qRot.Slerp(pKeySecond->qRot, t);
		}

		return qRot;
	}

	// render joint with bone ------------------------------------------------------------------
	//	v2에서 수정해야 하는 부분: 그려주는 joint와 다르게 숨겨진 joint들의 위치가 있으므로?
	//	그들의 위치는 어디에서 기록되는거지??
	void CJoint::renderBone(void)
	{
		GLboolean texture_enabled = GL_TRUE;
		glGetBooleanv(GL_TEXTURE_2D, &texture_enabled);
        glDisable(GL_TEXTURE_2D);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		Vector3d vZero(0.0f, 0.0f, 0.0f);
		Vector3d vThis(0.0f, 0.0f, 0.0f);
		vThis = mTransformMatrix * vThis;

		if (_mpParent)
		{
			Vector3d vParent(0.0f, 0.0f, 0.0f);

			CJoint* pParent	= _mpParent;
			assert(0 != pParent);

			vParent = pParent->getTransformMatrix() * vParent;

			glBegin(GL_LINES);
			{
				glVertex3f(vParent.x, vParent.y, vParent.z);
				glVertex3f(vThis.x, vThis.y, vThis.z);
			}
			glEnd();

			drawLink(vParent, vThis, _mMass/10);
			drawJoint(vThis, mDynamics.radius/8);
			//drawLink(_mpParent->getTransformMatrix(), mTransformMatrix, mDynamics.radius/50);
		}

		glPopMatrix();

		if (GL_TRUE == texture_enabled)
			glEnable(GL_TEXTURE_2D);
	}


	// draw link - 시작점부터 끝점까지 

	void	CJoint::drawLink(const CVector<double, 3>& _rStart,
				const CVector<double, 3>& _rEnd, double _radius)
	{
		const CVector<double, 3> length = _rEnd - _rStart;

		//double rz = (180. / 3.141592)* atan2(length.y, length.x);
		//double ry = 90. - (180. / 3.141592)* atan2(length.z , sqrt( length.x * length.x + length.y * length.y));
		//double height	= sqrt( length.x * length.x + length.y * length.y + length.z * length.z);

		double rz = (180. / 3.141592)* atan2(length.y, length.x);
		double ry = 90. - (180. / 3.141592)* atan2(length.z , sqrt( length.x * length.x + length.y * length.y));


		double rx = (180. / 3.141592)* atan2(length.y, length.z);
		double height	= sqrt( length.x * length.x + length.y * length.y + length.z * length.z);


//		glLoadIdentity();
//		glMatrixMode(GL_MODELVIEW);
		glTranslated(_rStart.x , _rStart.y , _rStart.z);
		glRotated( rz , 0. , 0. , 1.);
		glRotated( ry , 0. , 1. , 0.);
		//glRotated( rx , 1. , 0. , 0.);
		//glTranslated(length.x , length.y , length.z);


		oglCylinder( height , _radius);
	}

	void CJoint::drawLink(const CTransformMatrix& _rStart,
		const CTransformMatrix& _rEnd, double _radius)
	{
		CVector<double, 3> length;
		GLdouble	mStart[16]	= {
			_rStart.m11, _rStart.m12, _rStart.m13, _rStart.m14,
			_rStart.m21, _rStart.m22, _rStart.m23, _rStart.m24,
			_rStart.m31, _rStart.m32, _rStart.m33, _rStart.m34,
			_rStart.m41, _rStart.m42, _rStart.m43, _rStart.m44};

		GLdouble	mEnd[16]	= {
			_rEnd.m11, _rEnd.m12, _rEnd.m13, _rEnd.m14,
			_rEnd.m21, _rEnd.m22, _rEnd.m23, _rEnd.m24,
			_rEnd.m31, _rEnd.m32, _rEnd.m33, _rEnd.m34,
			_rEnd.m41, _rEnd.m42, _rEnd.m43, _rEnd.m44};


		length.x	= _rEnd.m14 - _rStart.m14;
		length.y	= _rEnd.m24 - _rStart.m24;
		length.z	= _rEnd.m34 - _rStart.m34;



		double rz = (180. / 3.141592)* atan2(length.y, length.x);
		double ry = 90. - (180. / 3.141592)* atan2(length.z , sqrt( length.x * length.x + length.y * length.y));
		double height	= sqrt( length.x * length.x + length.y * length.y + length.z * length.z);

		glMatrixMode(GL_MODELVIEW);
		glMultMatrixd(mStart);
		glTranslated(_rStart.m14, _rStart.m24, _rStart.m34);

		glMultMatrixd(mEnd);

		
//		glRotated( rz , 0. , 0. , 1.);
//		glRotated( ry , 0. , 1. , 0.);

		oglCylinder( height , _radius);
	};


	void	CJoint::drawJoint(const CVector<double, 3>& _rThis,
		double _radius)
	{
		switch(mDynamics.mType)
		{
		case PRISMATIC_JOINT:
		case FIXED_JOINT:
			glColor3f(1.f, 0.f, 0.f);
			oglSphere(_radius * 2.);
			break;

		case REVOLUTE_JOINT:
			glColor3f(0.f, 0.f, 1.f);
			oglSphere(_radius);
			break;
		}


		glColor3f(1.f, 1.f, 1.f);
	}


	bool	CJoint::isTouchedPlane(void)
	{
		if(mTransformMatrix.m24 <= 0.)
			return true;

		return false;
	}
}