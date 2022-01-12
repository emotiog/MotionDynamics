#pragma once
#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include "TransformMatrix.h"
#include "JointEnumerate.h"
#include "define.h"
#include "amcKeyframe.h"
#include "Sequence.h"
#include "MathMethods.h"
#include "DynamicMatrix.h"
#include <gl/glut.h>
#include "openGL.h"
#include "Constants.h"

namespace NAnimation
{
	typedef std::unordered_map< std::tstring, CSequence > NameSequenceMap;

	class CJoint
	{
	public:
		ENUM_ROTATION_TYPE	eRotOrder;

	protected:
		unsigned int		_mIndex;
		std::wstring		_mName;

		CJoint*					_mpParent;
		std::vector<CJoint*>	_mpChildren;

		double				_mLength;
		double				_mMass;

		CVector<double, 3>	_mLocalJointPos;		// 관절과 관절 사이 local position
		CVector<double, 3>	_mLocalJointDir;			// 관절과 관절 사이 회전된 정도
		CVector<double, 3>	_mLocalJointAxis;

		Matrix4x4d		_mPreRotation;
		Matrix4x4d		_mPostRotation;

		CVector<double, 3>			mCoM;	// local 질량 중심의 위치
	public:
		class CDOF
		{
		public:
			std::tstring		mAmcOrder[7];
			unsigned int		mAmcOrderCount;
			double				dLimits[7][2];

		public:
			CDOF(void);
			~CDOF(void);

		public:
			CDOF& operator=(const CDOF& _rSrc);
		}mDOF;

	public:
		void			setIndex(unsigned int _idx);
		unsigned int	getIndex(void) const;

		std::wstring	getName(void) const;
		void			setName(std::wstring _rStr);

		void			setParent(CJoint* _pPar);
		CJoint*			getParent(void) const;

		void			addChild(CJoint* _pChild);
		CJoint*			getChildren(void) const;
		CJoint*			getChild(unsigned int index = 0) const;

		void			setLength(double _rlngth);
		double			getLength(void) const;

		void			setMass(double _rMass);
		double			getMass(void) const;

		void				setLocalJointPosition(const CVector<double, 3>& _rPos);
		void				setLocalJointPosition(double _x, double _y, double _z);
		CVector<double,3>	getLocalJointPosition(void) const;

		void			setLocalJointDirection(double _x, double _y, double _z);

		void			setLocalJointAxis(const Vector3d& _rDir);
		void			setLocalJointAxis(double _x, double _y, double _z);
		
		CVector<double, 3>		getLocalJointAxis(void) const;
		CVector<double, 3>		getLocalJointDirection(void) const;

		CVector<double, 3>	getCoM(void)	const;


		const CTransformMatrix&		getTransformMatrix(void);
		const CTransformMatrix		TransformMatrix(void);
		const CTransformMatrix		TransformMatrixQ(void);
		const CTransformMatrix		TransformMatrixDiffQ(void);

	public:
		const CTransformMatrix		getTransformMatrixQ_quat(void);
		const CTransformMatrix		getTransformMatrixDiffQ_quat(void);



	public:
		bool			isName(void);

	public:
		CTransformMatrix		mLocalMat;
		CTransformMatrix		mTransformMatrix;
		CVector<double, 3>		mPreLocalPos;


	public:
		void updatePositionToTransformMatrix(void);
		void updateDirectionToTransformMatrix(void);

	public:
		CJoint(void);
		CJoint(const CJoint& _rJnt);

		~CJoint(void);

	public:
		CJoint& operator=(const CJoint& _rJnt);

	public:
		bool	insertAmcKeyframe(const std::tstring& sAmcFile,
					unsigned int nKeyframeIndex, const CAMCkeyframe& keyframe);

		void	activeSequence(const std::tstring& _rSequenceName);
		double	getAnimationStartTime(void) const;
		double	getAnimationStopTime(void) const;
		void	makeSmoothSequences(void);
		void	makeAnimationSequences(double fFPS);

	public:
		std::vector<CJoint*>	mHierarchyByCenter;
		std::vector<CJoint*>	getReverseHierarchy(void);

	public:
		class CJointMotion
		{
		public:
			CJoint*				mpInstance;
			std::unordered_map<std::tstring, std::vector<CAMCkeyframe>>
				_mMotionSequences;
			NameSequenceMap		_mSequences;
			CSequence*			_mActiveSequence;

			mutable unsigned int		lastPosKeyIndex_;
			mutable unsigned int		lastRotKeyIndex_;

		public:
			CJointMotion(void)
			{
				mpInstance			= NULL;
				_mMotionSequences.clear();
				_mSequences.clear();
				_mActiveSequence	= NULL;
				lastPosKeyIndex_	= 0;
				lastRotKeyIndex_	= 0;
			}

			CJointMotion(CJoint& _rJoint)
			{
				mpInstance			= &_rJoint;
				assert(mpInstance != NULL);

				_mMotionSequences	= _rJoint.cAnimation._mMotionSequences;
				_mSequences			= _rJoint.cAnimation._mSequences;
				_mActiveSequence	= _rJoint.cAnimation._mActiveSequence;
				lastPosKeyIndex_	= _rJoint.cAnimation.lastPosKeyIndex_;
				lastRotKeyIndex_	= _rJoint.cAnimation.lastRotKeyIndex_;
			}

			~CJointMotion(void)
			{
				mpInstance			= NULL;
				_mMotionSequences.clear();
				_mSequences.clear();
				_mActiveSequence	= NULL;
				lastPosKeyIndex_	= 0;
				lastRotKeyIndex_	= 0;
			}

			CJointMotion& operator=(const CJointMotion& _rSrc)
			{
				_mMotionSequences	= _rSrc._mMotionSequences;
				_mSequences			= _rSrc._mSequences;
				_mActiveSequence	= _rSrc._mActiveSequence;
				lastPosKeyIndex_	= _rSrc.lastPosKeyIndex_;
				lastRotKeyIndex_	= _rSrc.lastRotKeyIndex_;

				return (*this);
			}

		protected:
			CSequence* getSequenceByName(const std::tstring& _rSequenceName);

		public:
			void	makeAnimationSequences(const CDOF& _rDOF, ENUM_ROTATION_TYPE& _rType, double fFPS);
			void	activeSequence(const std::tstring& _rSequenceName);
			void	makeSmoothSequences(void);

			double	getAnimationStartTime(void) const;
			double	getAnimationStopTime(void) const;
		}cAnimation;

		public:
			const Matrix4x4d& getPreRotation(void) const;
			const Matrix4x4d& getPostRotation(void) const;

			CTransformMatrix	animate(double fCurTime);
			CTransformMatrix	animateRoot(double fCurTime,
				const Vector3d&		_worldPos = Vector3d(0, 0, 0),
				const Quaterniond&	_worldDir = Quaterniond(0. ,0. ,0. ,1.));
			Vector3d	getAnimatedPosition(double fCurTime) const;
			Quaterniond	getAnimatedRotation(double fCurTime) const;	


		public:
			void	animateWithDynamicQ(
				CTransformMatrix* _pWorldMat,
				CTransformMatrix*	_pLocalMat,
				Quaterniond* qRot = &Quaternion<double>(0., 0., 0., 1.),
				double fCurTime = 0.,
				const CVector<double, 3>& _rScale = CVector<double,3>(1., 1. ,1.),
				const Vector3d&		_worldPos = Vector3d(0.f, 0.f, 0.f),
				const Quaterniond&	_worldDir = Quaterniond(0.f, 0.f, 0.f, 1.f));
			
			//void	animateRootWithDynamicQ(
			//	CTransformMatrix* _pWorldMat,
			//	CTransformMatrix*	_pLocalMat,
			//	Quaterniond* qRot = &Quaternion<double>(0., 0., 0., 1.),
			//	double fCutTime = 0.,
			//	const CVector<double, 3>& _rScale = CVector<double, 3>(1., 1., 1.),
			//	const Vector3d&		_worldPos = Vector3d(0.f, 0.f, 0.f),
			//	const Quaterniond&	_worldDir = Quaterniond(0.f, 0.f, 0.f, 1.f));
			//
			//void	computeForce(CJoint*	_pStart,
			//	CTransformMatrix* _pWorldMat,
			//	CTransformMatrix*	_pLocalMat,
			//	Quaterniond* qRot = &Quaternion<double>(0., 0., 0., 1.),
			//	double fCurTime = 0.,
			//	const Vector3d&		_vTransPos = Vector3d(0.f, 0.f, 0.f),
			//	const Quaterniond&	_vRotateDir = Quaterniond(0.f, 0.f, 0.f, 1.f)
			//	);

			void	standFoot(CJoint*	_pStart,	CJoint*	_pChild,
				CTransformMatrix* _pWorldMat,
				CTransformMatrix*	_pLocalMat,
				double fCurTime = 0.,
				const CVector<double, 3>& _rScale = CVector<double, 3>(1., 1., 1.),
				const Vector3d&		_vTransPos = Vector3d(0.f, 0.f, 0.f),
				const Quaterniond&	_vRotateDir = Quaterniond(0.f, 0.f, 0.f, 1.f)
				);

			void	standFoot(CTransformMatrix* _pWorldMat,	CTransformMatrix*	_pLocalMat);
			void	updateMatrixByDynamics(CTransformMatrix* _pWorldMat,	
				CTransformMatrix*	_pLocalMat,
				const CJoint*	_pChild,
				const CVector<double, 3>& _rScale);

		protected:
			void	drawLink(const CVector<double, 3>& _rStart,
				const CVector<double, 3>& _rEnd, double _radius);
			
			void	drawLink(const CTransformMatrix& _rStart,
				const CTransformMatrix& _rEnd, double _radius);

			void	drawJoint(const CVector<double, 3>& _rThis,
				double _radius);

		public:
			void	renderBone(void);	

		public:
			bool	isTouchedPlane(void);


		public:
			class CDynamics
			{
			public:
				int		mAxis;				// 회전(직선) 운동 중심 축 지정
											// (0 - x축 1 - y축, 2 - z축)

				ENUM_JOINT_TYPE	mType;
				CVector<double, 3>			mLocalDir;	// 내가 가고자 하는 방향, 혹은 회전 축
				//double phi, theta, psi;		// 이전 조인트 자세로부터 현재 조인트의 상대 자세
				double radius;				// 링크의 굵기, OpenGL에서 그리기 위해서 사용함

				// 프로그램 실행 중 사용되는 변수들
				double q;					// 조인트의 현재 회전(직선) 위치
				double dq, ddq;

				CMatrix<double, 3, 3>		R;	// world 좌표계에서 joint의 회전 matrix
				//CDynamicVector<double>		a;	// world 좌표계에서의 z-axis의 방향 단위 벡터
				CVector<double, 3>			mTwistAxis;	// world 좌표계에서의 z-axis의 방향 단위 벡터
				CVector<double, 3>			mWorldPos;	// world 좌표계에서의 joint 위치
				//CDynamicVector<double>		g;		// world 좌표계에서 joint의 질량중심 위치
				CVector<double, 3>			mWorldCOM;

				std::vector<CVector<double, 3>> dWorldAxis;				// 회전 axis의 q(k th joint)에 대한 편미분 값
				std::vector<CVector<double, 3>> dWorldPos;				// world 위치의 q(k th joint)에 대한 편미분 값
				std::vector<CVector<double, 3>> dWorldCoM;				// 질량 중심의 q(k th joint)에 대한 편미분 값
				//std::vector<CDynamicVector<double>> dg;				

				std::vector<CMatrix<double, 3, 3>>	dR;	// R의 q(k)에 대한 편미분 값

				CDynamics& operator=(const CDynamics& _rSrc);

			public:
				//CDynamicMatrix<double>	I;	// joint의 관성행렬
				CMatrix<double, 3, 3>		I;	// Ixx, Iyy, Izz

			public:
				CDynamics(void);

			public:
				void arrangeQ(void);

			}mDynamics;
	};

}