#ifndef __MOTION_LOADER_H_
#define __MOTION_LOADER_H_

#include "Skeleton.h"
#include <map>
#include "define.h"
#include "Character.h"
#include "JointEnumerate.h"
#include "Constants.h"
#include "amcKeyframe.h"
#include "Hierarchy.h"

namespace NAnimation
{
	typedef std::vector<std::wstring>	StringArray;
	typedef std::vector<CJoint>			JointArray;

	class CMotionLoader
	{
	public:
		std::wstring	mFilePath;
		std::wstring	mName;
		CSkeleton*		mpBody;
		double			mGlobalLengthMultiplier;
		bool			mDegree;

		StringArray		mExceptedBodyPart;
		CHierarchy		mHierarchy;

	public:
		CMotionLoader(void);
		CMotionLoader(CSkeleton* _pBody);
		~CMotionLoader(void);

		void clear(void);

	public:
		bool connectSkeleton(CSkeleton* _pBody);
		bool loadAcclaimMotionFile(const std::wstring& _rFilePath);
		bool loadAcclaimSkeletonFile(const std::wstring& _rFilePath);

	protected:
		void _skipEmptyChar(std::tistream& inStream);
		void _skipSpaceChar(std::tistream& inStream);

	protected:
		bool parseAsfVersion(std::tifstream& fin);
		bool parseAsfName(std::tifstream& fin);
		bool parseAsfUnits(std::tifstream& fin);
		bool parseAsfDocumentation(std::tifstream& fin);
		bool parseAsfRoot(std::tifstream& fin);
		bool parseAsfBoneData(std::tifstream& fin);
		bool parseAsfHierarchy(std::tifstream& fin, std::map< std::tstring, TStringArray >& hierarchy);

		void parseAsfAmcOrder(const std::tstring& sAmcOrder, CJoint& asf_joint);

		bool parseAmcKeyframe(std::tifstream& fin, std::vector< CAMCkeyframe >& motionSequence);
		
		bool _insertAmcKeyframe(const std::tstring& sAmcFile,
		unsigned int nKeyframeIndex, std::vector< CAMCkeyframe >& _rMotionSequences);

		Vector3d _convertAngleToDegree(const Vector3d& _rDir);
				
	private:
		void	_setJointType(const CHierarchy& _rHierarchy);

	protected:
		bool		_checkValidation(void);	// check if it has only 1 root or not.
		void		_bodySimplify(const CHierarchy& _rHierarchy);
		bool		_connectByHierarchy(CHierarchy* _pHierarchy);
	};
}


#endif