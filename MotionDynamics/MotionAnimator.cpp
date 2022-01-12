#include "StdAfx.h"
#include "MotionAnimator.h"

namespace NAnimation
{
	CMotionAnimator::CMotionAnimator(void)
	{
		mGravity.x	= 0.f;
		mGravity.y	= -9.81f;
		mGravity.z	= 0.f;

		mpBody	= NULL;
		mFsi	= 0;
		mFvi	= 0;

		m_bTagged	= false;
	}


	CMotionAnimator::~CMotionAnimator(void)
	{
	}
	

	CMotionAnimator::CMotionAnimator(CSkeleton* _pBody)
	{
		mpBody = _pBody;
	}


	bool	CMotionAnimator::connectSkeleton(CSkeleton* _pBody)
	{
		if(_pBody == NULL)
			return false;

		if(mpBody != NULL)
			return false;

		mpBody	= _pBody;

		return true;
	}

	// ---- Jacobian Pos Static -------------------------------------------------
	// root는 translate하자
	// ith의 부모가 root 일 때까지 혹은 thorax일 때까지.
	// 혹은 ith의 부모가 endNode일 때까지.
	CMatrix<double, 3, 15>	CMotionAnimator::jacobianPosStatic(unsigned int ith)
	{
		const unsigned int jaSize	 = 15;
		
		CMatrix<double, 3, jaSize>	Jp;

		CJoint*	tarJoint	= mpBody->getIndex(ith);
			
#ifdef _SKELETON
		for(unsigned int _iter = 0 ; _iter < tarJoint->mHierarchyByCenter.size() ; _iter ++)
		{
			CJoint* curJoint	= tarJoint->mHierarchyByCenter[_iter];
#else
		for( unsigned int _iter = 0 ; _iter < jaSize && _iter < ith ; _iter ++)
		{
			CJoint* curJoint	= mpBody->getIndex(_iter);

#endif
			int	influenceIndex	= curJoint->getIndex();

			if(curJoint->mDynamics.mType == REVOLUTE_JOINT) 
			{
				CVector<double, 3> pos;

				pos = curJoint->mDynamics.mTwistAxis.crossProduct( 
					tarJoint->mDynamics.mWorldCOM 
					- curJoint->mDynamics.mWorldPos);	// 현재 관절 위치 + dynamic 변형 위치

				Jp.elements[0][influenceIndex]	= pos.x;
				Jp.elements[1][influenceIndex]	= pos.y;
				Jp.elements[2][influenceIndex]	= pos.z;
			}
			else if(curJoint->mDynamics.mType == PRISMATIC_JOINT)
			{
				Jp.elements[0][influenceIndex]	= curJoint->mDynamics.mTwistAxis[0];
				Jp.elements[1][influenceIndex]	= curJoint->mDynamics.mTwistAxis[1];
				Jp.elements[2][influenceIndex]	= curJoint->mDynamics.mTwistAxis[2];
			}
		}

		return Jp;
	}


	CMatrix<double, 3, 15> CMotionAnimator::jacobianOriStatic(unsigned int ith)
	{
		assert(mpBody != NULL);
		const unsigned int size	= 15;

		//CDynamicMatrix<double>	Jo(3, size);
		CMatrix<double, 3, 15> Jo;
		
		CJoint*	tarJoint	= mpBody->getIndex(ith);
#ifdef _SKELETON
		for(unsigned int _iter = 0 ; _iter < tarJoint->mHierarchyByCenter.size() ; _iter ++)
		{
			CJoint* curJoint	= tarJoint->mHierarchyByCenter[_iter];
#else
		for( unsigned int _iter = 0 ; _iter < size && _iter < ith ; _iter ++)
		{
			CJoint* curJoint	= mpBody->getIndex(_iter);

#endif
			int	influenceIndex	= curJoint->getIndex();

			if(curJoint->mDynamics.mType	== REVOLUTE_JOINT)
			{
				Jo.elements[0][influenceIndex]	= curJoint->mDynamics.mTwistAxis[0];
				Jo.elements[1][influenceIndex]	= curJoint->mDynamics.mTwistAxis[1];
				Jo.elements[2][influenceIndex]	= curJoint->mDynamics.mTwistAxis[2];
			}

			else if( curJoint->mDynamics.mType	== PRISMATIC_JOINT)
			{
				Jo(0, influenceIndex)	= 0.;
				Jo(1, influenceIndex)	= 0.;
				Jo(2, influenceIndex)	= 0.;
			}
		}

		return Jo;
	}


	CMatrix<double, 3, 15> CMotionAnimator::jacobianPosDiffStatic(unsigned int li, unsigned int k_thJoint)
	{
		assert(mpBody != NULL);

		CMatrix<double, 3, 15> dJp;

		CJoint*	tarJoint	= mpBody->getIndex(li);

#ifdef _SKELETON
		for(unsigned int _iter = 0 ; _iter < tarJoint->mHierarchyByCenter.size() ; _iter ++)
		{
			CJoint* curJoint	= tarJoint->mHierarchyByCenter[_iter];
#else
		const int size	= mpBody->getJointSize();

		for( unsigned int _iter = 0 ; _iter < size && _iter < li ; _iter ++)
		{
			CJoint* curJoint	= mpBody->getIndex(_iter);

#endif
			int	influenceIndex	= curJoint->getIndex();

			if(curJoint->mDynamics.mType	== REVOLUTE_JOINT)
			{// li이 해당 관절 세부 파트 밖에 있을 경우 문제가 있음
				CVector<double, 3> LinkEnd	= tarJoint->mDynamics.mWorldCOM 
					- curJoint->mDynamics.mWorldPos;
				CVector<double, 3> dLinkEnd	= tarJoint->mDynamics.dWorldCoM[k_thJoint] 
					- curJoint->mDynamics.dWorldPos[k_thJoint];
				
				CVector<double, 3> pos = curJoint->mDynamics.dWorldAxis[k_thJoint].crossProduct( LinkEnd) 
					+ curJoint->mDynamics.mTwistAxis.crossProduct(dLinkEnd);

				dJp.elements[0][influenceIndex]	= pos.x;
				dJp.elements[1][influenceIndex]	= pos.y;
				dJp.elements[2][influenceIndex]	= pos.z;
			}
			else if(curJoint->mDynamics.mType	== PRISMATIC_JOINT)
			{
				dJp.elements[0][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][0];
				dJp.elements[1][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][1];
				dJp.elements[2][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][2];
			}
		}

		return dJp;
	}


	CMatrix<double, 3, 15>	CMotionAnimator::jacobianOriDiffStatic(unsigned int li,
		unsigned int k_thJoint)
	{
		assert(mpBody != NULL);

		CMatrix<double, 3, 15>	dJo;

		CJoint*	tarJoint	= mpBody->getIndex(li);

		//for( unsigned int _iter = 0 ; _iter < size  && _iter < li ; _iter ++)
		//{
#ifdef _SKELETON
		for(unsigned int _iter = 0 ; _iter < tarJoint->mHierarchyByCenter.size() ; _iter ++)
		{
			CJoint* curJoint	= tarJoint->mHierarchyByCenter[_iter];
#else
		
		const unsigned int size	= mpBody->getJointSize();

		for( unsigned int _iter = 0 ; _iter < size && _iter < li ; _iter ++)
		{
			CJoint* curJoint	= mpBody->getIndex(_iter);

#endif
			int	influenceIndex	= curJoint->getIndex();

			if(curJoint->mDynamics.mType	== PRISMATIC_JOINT)
			{
				dJo.elements[0][influenceIndex]	= 0.;
				dJo.elements[1][influenceIndex]	= 0.;
				dJo.elements[2][influenceIndex]	= 0.;
			}
			else if(curJoint->mDynamics.mType	== REVOLUTE_JOINT)
			{
				dJo.elements[0][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][0];
				dJo.elements[1][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][1];
				dJo.elements[2][influenceIndex]	= curJoint->mDynamics.dWorldAxis[k_thJoint][2];
			}
		}

		return dJo;
	}



	void	CMotionAnimator::inertiaMatrix15x15(CMatrix<double, 15, 15>* _pMat,
		const std::vector<CMatrix<double, 3, 15>> &Jp,
		const std::vector<CMatrix<double, 3, 15>>& Jo)
	{
		assert(_pMat != NULL);

		_pMat->zero();
		
		const unsigned int size	= mpBody->getJointSize();
		
		for(unsigned int kth = 0 ; kth < size ; ++ kth)
		{
			CJoint* joint	= mpBody->getIndex(kth);

			*_pMat += joint->getMass() * ~Jp[kth] * Jp[kth] + 
				(~Jo[kth] *joint->mDynamics.R) *joint->mDynamics.I * ~joint->mDynamics.R * Jo[kth];
		}
	}


	void CMotionAnimator::coriolisMatrix15x15(CMatrix<double, 15, 15>* _pMat,
		const std::vector<CMatrix<double, 3, 15>> &Jp,
		const std::vector<CMatrix<double, 3, 15>>& Jo)
	{
		assert(_pMat != NULL);
		
		const unsigned int size	= mpBody->getJointSize();

		std::vector<CMatrix<double, 15, 15>> dM(15);

		for(unsigned int k_ = 0 ; k_ < size ; ++ k_)
		{
			CJoint* joint = mpBody->getIndex(k_);
			
			CMatrix<double, 3, 3>	IR	= joint->mDynamics.R * joint->mDynamics.I * ~joint->mDynamics.R;
			CMatrix<double, 3, 3>	dIR	= joint->mDynamics.dR[k_] * joint->mDynamics.I * ~joint->mDynamics.R 
				+ joint->mDynamics.R * joint->mDynamics.I * ~joint->mDynamics.dR[k_];

			dM[k_].zero();

			for(unsigned int i_ = 0 ; i_ < size ; i_ ++)
			{
				CMatrix<double, 3, 15> dJp	= jacobianPosDiffStatic( i_, k_);
				CMatrix<double, 3, 15> dJo	= jacobianOriDiffStatic( i_, k_);

				dM[k_] += joint->getMass() * ( ~dJp * Jp[ i_] + ~Jp[ i_] *dJp)
					+ ~dJo * IR * Jo[i_] + ~Jo[i_] *dIR * Jo[i_] + ~Jo[i_] * IR * dJo;
			}
		}

		_pMat->zero();

		for(unsigned int i_ = 0 ; i_ < 15 ; ++ i_)
		{
			for(unsigned int j_ = 0 ; j_ < 15 ; j_ ++)
			{
				for(unsigned int k_th = 0 ; k_th < size ; k_th ++)
				{
					_pMat->elements[i_][j_] += (dM[k_th].elements[i_][j_] 
					+ dM[j_ ].elements[i_][k_th] - dM[i_ ].elements[j_][k_th]) 
						* mpBody->getIndex(k_th)->mDynamics.dq; 
				}
				_pMat->elements[i_][j_]	*= 0.5f;
			}
		}
	}


	void	CMotionAnimator::gravityMatrix15x1(CVector<double, 15>* _pVec,
		const std::vector<CMatrix<double, 3, 15>> &Jp,
		const std::vector<CMatrix<double, 3, 15>> &Jo)
	{
		assert(_pVec != NULL);
				
		const unsigned int size	= mpBody->getJointSize();

		_pVec->zero();

		for(unsigned int i_ = 0 ; i_ < size ; ++ i_)
		{
			CJoint* joint	= mpBody->getIndex(i_);

			for(unsigned int j_ = 0 ; j_ < size ; ++ j_)
			{
				CJoint* joint2	= mpBody->getIndex(j_);

				CVector<double, 3>	Jpc;

				Jpc.x	= Jp[j_](0, i_);
				Jpc.y	= Jp[j_](1, i_);
				Jpc.z	= Jp[j_](2, i_);

				CVector<double, 3>	scaledGravity;
				
				scaledGravity.x = mGravity.x * joint2->getMass();
				scaledGravity.y = mGravity.y * joint2->getMass();
				scaledGravity.z = mGravity.z * joint2->getMass();

				double	gi	= scaledGravity.dotProduct(Jpc);
				_pVec->elements[i_]	-= gi;
			}
		}
	}


	void	CMotionAnimator::frictionForce15x1(CVector<double, 15>* _friction)
	{				
		const unsigned int size	= mpBody->getJointSize();

		for(unsigned int i_ = 0 ; i_ < size ; ++ i_)
		{
			_friction->elements[i_] = mFsi * sign(mpBody->getIndex(i_)->mDynamics.dq)
				+ mFvi * mpBody->getIndex(i_)->mDynamics.dq;
		}
	}


	double CMotionAnimator::kineticEnergyStatic(const CMatrix<double, 15, 15>& M)
	{
		CMatrix<double, 15, 1> dq;

		for(unsigned int i_ = 0 ; i_ < 15 ; ++ i_)
		{
			dq.elements[i_][0]	= mpBody->getIndex(i_)->mDynamics.dq;
		}

		CMatrix<double, 1,1>	a	= 0.5f * (~dq * M * dq);

		return a(0, 0);
	}

	double CMotionAnimator::potentialEnergy(void)
	{
		//CDynamicMatrix<double> b(1,1);
		double	b	= 0.f;

		//b.null();

		for(unsigned int i_ = 0 ; i_ < mpBody->getJointSize() ; ++ i_)
		{
			//b -= mpBody->getIndex(i_)->getMass() * 
			//	mGravity * mpBody->getIndex(i_)->mDynamics.g;
			b	-= mpBody->getIndex(i_)->getMass() * 
				mGravity.dotProduct(mpBody->getIndex(i_)->mDynamics.mWorldCOM);
		}

		return b;//(0,0);
	}


	double	CMotionAnimator::getKineticEnergy(void) const
	{
		return _mKineticEnergy;
	}


	double	CMotionAnimator::getPotentialEnergy(void) const
	{
		return _mPotentialEnergy;
	}


	void CMotionAnimator::reset(void)
	{
		if(mpBody == NULL)
			return;

		if(mpBody->emptyJoint())
		{
			return;
		}

		for(unsigned int iter_ = 0 ; iter_ < mpBody->getJointSize() ; iter_ ++)
		{
			mpBody->getIndex(iter_)->mDynamics.q	= 0.;
			mpBody->getIndex(iter_)->mDynamics.dq	= 0.;
			mpBody->getIndex(iter_)->mDynamics.ddq	= 0.;
		}

		mGravity.x	= 0.f;
		mGravity.y	= -9.81f;
		mGravity.z	= 0.f;
		
		mFsi	= 0.;
		mFvi	= 0.;
		
		mConstraintAccel.zero();
	};


	double CMotionAnimator::_terminalSpeed(double _speed)
	{
		if(_speed > PI_D)
			return PI_D;
		
		else if(_speed < -PI_D)
			return -PI_D;

		else
			return _speed;
	}


	void CMotionAnimator::applyStatic15x1(const CVector<double, 15>& _rAccel, double dt)
	{
		const unsigned int size = mpBody->getJointSize();

		for(unsigned int iter_ = 0 ; iter_ < size ; iter_ ++)
		{
			CJoint* joint	= mpBody->getIndex(iter_);
			joint->mDynamics.ddq		= _rAccel[iter_];		// += 이 아니라?

			joint->mDynamics.dq	+= dt * joint->mDynamics.ddq;
			//joint->mDynamics.dq = _terminalSpeed(joint->mDynamics.dq);

			joint->mDynamics.q	+= dt *	joint->mDynamics.dq;
		}
	}


	CVector<double, 15>	CMotionAnimator::forwardDynamicsStatic(const CVector<double, 15>& _rTorque)
	{
		std::vector<CMatrix<double, 3, 15>> Jo;
		std::vector<CMatrix<double, 3, 15>> Jp;

		
		const unsigned int size	= mpBody->getJointSize();

		for(unsigned int iter_ = 0 ; iter_ < size ; iter_ ++)
		{
			Jp.push_back(jacobianPosStatic(iter_));
			Jo.push_back(jacobianOriStatic(iter_));
		}


		CMatrix<double, 15, 15>	M;
		inertiaMatrix15x15(&M, Jp, Jo);
		
		CMatrix<double, 15, 15>	C;
		coriolisMatrix15x15(&C, Jp, Jo);

		CVector<double, 15>		G;
		gravityMatrix15x1(&G, Jp, Jo);


		CVector<double, 15>	F;
		frictionForce15x1(&F);

		//_mKineticEnergy		= kineticEnergyStatic(M);
		//_mPotentialEnergy	= potentialEnergy();


		CVector<double, 15> dq;

		for(unsigned int iter_	= 0 ; iter_ < size ; ++ iter_)
		{
			dq[iter_]	= mpBody->getIndex(iter_)->mDynamics.dq;
		}

		double isLambda	= 0.0001f;

		CMatrix<double, 15, 15> I;

		CMatrix<double, 15, 15> invM	= ~M * !( M * ~M + isLambda * isLambda * I);

		return invM * (_rTorque - (C *dq + G + F));
	}




	// ----	Inverse Dynamics
	//	힘을 통해서 내가 어느 방향으로 가고 있는지 나오는 것
	//	그 방향의 반대 방향으로 움직여야 겠지?..
	CVector<double, 15>	CMotionAnimator::inverseDynamicsStatic(void)
	{
		const unsigned int size = 15;
		const unsigned int theNum = mpBody->getJointSize();

		std::vector<CMatrix<double, 3, size>> Jo;
		std::vector<CMatrix<double, 3, size>> Jp;

		switch(mpBody->m_ePushedDir)	// 왼쪽으로 밀었을 때
		{
		case PUSHED_ROOT:	
			break;
		case PUSHED_LEFT:
			break;
		case PUSHED_RIGHT:
			break;
		}

		for(unsigned int iter_ = 0 ; iter_ < theNum ; ++ iter_)
		{
			Jp.push_back(jacobianPosStatic(iter_));
			Jo.push_back(jacobianOriStatic(iter_));
		}

		CMatrix<double, size, size>	M;
		inertiaMatrix15x15(&M, Jp, Jo);
		
		CMatrix<double, size, size>	C;
		coriolisMatrix15x15(&C, Jp, Jo);

		CVector<double, size>		G;
		gravityMatrix15x1(&G, Jp, Jo);

		CVector<double, size>		F;
		frictionForce15x1(&F);

		//_mKineticEnergy		= kineticEnergyStatic(M);
		//_mPotentialEnergy	= potentialEnergy();

		CVector<double, size>	dq;
		CVector<double, size>	ddq;

		for(unsigned int iter_ = 0 ; iter_ < size ; ++ iter_)
		{
			dq[iter_]	= mpBody->getIndex(iter_)->mDynamics.dq;
			ddq[iter_]	= mpBody->getIndex(iter_)->mDynamics.ddq;
		}

		return M * ddq + C *dq + G + F;
	}


	void	CMotionAnimator::setReferenceBody(void)
	{
		m_SkeleRef	= *mpBody;
		m_bTagged	= true;
	}


	// ---- 모든 관절과 거리를 비교하여 그에 따라 일반화 힘을 비례하여 부여
	void	CMotionAnimator::jointsDistance(CVector<double, 15> *_pForce)
	{
		assert(m_bTagged);
		assert(!m_SkeleRef.emptyJoint());

		const int	size	= 15;
		
		CJoint*	pOrig	= NULL;
		CJoint* pRef	= NULL;

		CVector<double, 3>	worldPosDis;

		for(unsigned int _iter = 0 ; _iter < size ; _iter ++)
		{
			pOrig	= mpBody->getIndex(_iter); 
			pRef	= m_SkeleRef.getIndex(_iter);

			// 내 위치가 -> ref의 위치로 가기 위해서 : ref 위치 - 내 위치
			worldPosDis = pRef->mDynamics.mWorldPos - pOrig->mDynamics.mWorldPos ;

			// 3차원의 world Position 의 distance를 어떻게 1차원의 회전 값으로
			// 비례상수로 만들 수 있을까?
			_pForce->elements[_iter]	= fabs(worldPosDis.x) + fabs(worldPosDis.y) + fabs(worldPosDis.z);
				//(worldPosDis.x * worldPosDis.x) + (worldPosDis.y * worldPosDis.y) +
				//(worldPosDis.z * worldPosDis.z);
				//.length();	// 의미 있게 선형 값을 회전 값으로 대입하고 싶소
		}
	}
}