#ifndef __CMOTION_ANIMATIOR_H_
#define __CMOTION_ANIMATOR_H_

#include "Skeleton.h"
#include "DynamicMatrix.h"
#include "matrixConverter.h"


// CMotionAnimator - Dynamics 와 관련하여 Animator를 
namespace NAnimation
{
	typedef std::vector<CVector<double, 3>>  VariableMatrix;
	
	class CMotionAnimator
	{
	public:
		CSkeleton*	mpBody;
		
		CSkeleton	m_SkeleRef;
		bool		m_bTagged;	// skeleRef가 이미 복사되었다

		void	setReferenceBody(void);
		void	jointsDistance(CVector<double, 15> *_pForce);

	public:
		CMotionAnimator(void);
		~CMotionAnimator(void);

		CMotionAnimator(CSkeleton* _pBody);
		CMotionAnimator& operator=(const CMotionAnimator& _rSrc);

		bool	connectSkeleton(CSkeleton* _pBody);

	public:
		CVector<double, 3>		mGravity;
		double					mFsi;		// coulomb friction coefficient
		double					mFvi;		// viscous friction coefficient
		CVector<double, 15>		mConstraintAccel;
		CVector<double, 15>		mDistanceForce;

	public:
		CVector<double, 15>		forwardDynamicsStatic(const CVector<double, 15>& _rTorque);
		CVector<double, 15>		inverseDynamicsStatic(void);


	protected:
		CMatrix<double, 3, 15>	jacobianPosStatic(unsigned int ith);
		CMatrix<double, 3, 15>	jacobianOriStatic(unsigned int ith);

		CMatrix<double, 3, 15>	jacobianPosDiffStatic(unsigned int li, unsigned int k_thJoint);
		CMatrix<double, 3, 15>	jacobianOriDiffStatic(unsigned int li, unsigned int k_thJoint);

	protected:
		double	_mKineticEnergy;
		double	_mPotentialEnergy;

	protected:
		void	inertiaMatrix15x15(CMatrix<double, 15, 15>* _pMat, 
			const std::vector<CMatrix<double, 3, 15>> &Jp,
			const std::vector<CMatrix<double, 3, 15>>& Jo);

		void	coriolisMatrix15x15(CMatrix<double, 15, 15>* _pMat,
			const std::vector<CMatrix<double, 3, 15>> &Jp,
			const std::vector<CMatrix<double, 3, 15>>& Jo);

		void	gravityMatrix15x1 (CVector<double, 15>* _pVec,
			const std::vector<CMatrix<double, 3, 15>> &Jp,
			const std::vector<CMatrix<double, 3, 15>>& Jo);

		void	frictionForce15x1(CVector<double, 15>* _friction);


		double	kineticEnergyStatic(const CMatrix<double, 15, 15>& M);

	public:
		double	potentialEnergy(void);

		void	reset(void);
		void	applyStatic15x1(const CVector<double, 15>& _rAccel, double dt = 0.01);


	public:
		double	getKineticEnergy(void)		const;
		double	getPotentialEnergy(void)	const;

	protected:
		double	_terminalSpeed(double _speed);
	};

}

#endif