#include "stdafx.h"
#include "Sequence.h"

namespace NAnimation
{
	PosKeyArray	CSequence::posKeyBuffer;
	RotKeyArray	CSequence::rotKeyBuffer;
	unsigned int CSequence::rotKeyBufferSize	= 0;
	unsigned int CSequence::rotKeyBufferCount	= 0;
	unsigned int CSequence::posKeyBufferSize	= 0;
	unsigned int CSequence::posKeyBufferCount	= 0;

	CSequence::CSequence(void)
		: lastPosKeyIndex_(0)
		, lastRotKeyIndex_(0)
		, currentTime_(0.0f)
		, currentRotation_(0.0f, 0.0f, 0.0f, 1.0f)
		, currentPosition_(0.0f, 0.0f, 0.0f)
	{
	}


	CSequence::~CSequence(void)
	{
		destroy();
	}


	CSequence::CSequence(const CSequence& rhs)
		: rotKeys_(rhs.rotKeys_)
		, posKeys_(rhs.posKeys_)
		, lastPosKeyIndex_(rhs.lastPosKeyIndex_)
		, lastRotKeyIndex_(rhs.lastRotKeyIndex_)
		, currentTime_(rhs.currentTime_)
		, currentRotation_(rhs.currentRotation_)
		, currentPosition_(rhs.currentPosition_)
	{
	}

	CSequence::CSequence(CSequence&& rhs)
	{
		(*this) = std::move(rhs);
	}


	CSequence& CSequence::operator=(CSequence&& rhs)
	{
		if (this != &rhs)
		{
			rotKeys_.clear();
			rotKeys_.swap(rhs.rotKeys_);

			posKeys_.clear();
			posKeys_.swap(rhs.posKeys_);

			lastPosKeyIndex_ = rhs.lastPosKeyIndex_;
			lastRotKeyIndex_ = rhs.lastRotKeyIndex_;
			currentTime_ = rhs.currentTime_;
			currentRotation_ = rhs.currentRotation_;
			currentPosition_ = rhs.currentPosition_;
		}

		return (*this);
	}


	CSequence& CSequence::operator=(const CSequence& rhs)
	{
		if (this != &rhs)
		{
			rotKeys_ = rhs.rotKeys_;
			posKeys_ = rhs.posKeys_;

			lastRotKeyIndex_ = rhs.lastRotKeyIndex_;
			lastPosKeyIndex_ = rhs.lastPosKeyIndex_;
			currentTime_ = rhs.currentTime_;
			currentRotation_ = rhs.currentRotation_;
			currentPosition_ = rhs.currentPosition_;
		}
		return (*this);
	}


	bool CSequence::createFromAmcKeyframes(const std::tstring _rAMCorder[7], 
			const unsigned int _rAMCcount, 
			ENUM_ROTATION_TYPE _rRotOrder,
		const std::vector<CAMCkeyframe>& keyframes, 
		float fFramesPerSecond)
	{
		destroy();

		CPosKey pos_key;
		CRotKey rot_key;

		for (size_t i = 0; i < keyframes.size(); ++i)
		{
			const CAMCkeyframe& keyframe = keyframes[i];

			const float fTime = static_cast<float>(keyframe.mKeyIndex) / fFramesPerSecond;

			bool bContainedRot = false;
			bool bContainedPos = false;
			Vector3d vPosition(0.0f, 0.0f, 0.0f);
			Vector3d vEuler(0.0f, 0.0f, 0.0f);

			for (unsigned int iter = 0; iter < _rAMCcount; ++iter)
			{
				if (_T("TX") == _rAMCorder[iter])
				{
					//vPosition.x = static_cast<float>(keyframe.dValues[i]);
					vPosition.x = 0.0f;
					bContainedPos = true;
				}
				else if (_T("TY") == _rAMCorder[iter])
				{
					//vPosition.y = static_cast<float>(keyframe.dValues[i]);
					bContainedPos = true;
				}
				else if (_T("TZ") == _rAMCorder[iter])
				{
					//vPosition.z = static_cast<float>(keyframe.dValues[i]);
					vPosition.z = 0.0f;
					bContainedPos = true;
				}
				else if (_T("RX") == _rAMCorder[iter])
				{
					vEuler.x = static_cast<float>(keyframe.mValues[iter]);
					bContainedRot = true;
				}
				else if (_T("RY") == _rAMCorder[iter])
				{
					vEuler.y = static_cast<float>(keyframe.mValues[iter]);
					bContainedRot = true;
				}
				else if (_T("RZ") == _rAMCorder[iter])
				{
					vEuler.z = static_cast<float>(keyframe.mValues[iter]);
					bContainedRot = true;
				}
				else
					assert(false);
			}

			// Insert information
			if (rotKeys_.empty() && bContainedRot)
				
			if (posKeys_.empty() && bContainedPos)
				posKeys_.reserve(keyframes.size());

			if (bContainedPos)
			{
				if (posKeys_.empty())
					posKeys_.reserve(keyframes.size());

				pos_key.fTime = fTime;
				pos_key.vPos = vPosition;
				posKeys_.push_back(pos_key);
			}

			if (bContainedRot)
			{
				if (rotKeys_.empty())
					rotKeys_.reserve(keyframes.size());

				rot_key.fTime = fTime;
				rot_key.qRot = MakeQuaterniondFromEulerRadianAngles(vEuler, _rRotOrder);
				rotKeys_.push_back(rot_key);
			}
		}

		return true;
	}


	void CSequence::smoothing(void)
	{
		if (rotKeys_.size() > 4)
		{
			RotKeyArray new_rot_keys(rotKeys_.size()/2 - 1);
			for (size_t i = 0; i < new_rot_keys.size(); ++i)
			{
				new_rot_keys[i].fTime =
					rotKeys_[i*2 + 0].fTime +
					rotKeys_[i*2 + 1].fTime +
					rotKeys_[i*2 + 2].fTime;
				new_rot_keys[i].fTime /= 3.0f;

				Quaterniond q0 = rotKeys_[i*2 + 0].qRot.Slerp(rotKeys_[i*2 + 1].qRot, 0.5f);
				Quaterniond q1 = rotKeys_[i*2 + 1].qRot.Slerp(rotKeys_[i*2 + 2].qRot, 0.5f);

				new_rot_keys[i].qRot = q0.Slerp(q1, 0.5f);
			}
			rotKeys_.swap(new_rot_keys);
		}

		if (posKeys_.size() > 4)
		{
			PosKeyArray new_pos_keys(posKeys_.size() / 2 - 1);
			for (size_t i = 0; i < new_pos_keys.size(); ++i)
			{
				new_pos_keys[i].fTime =
					posKeys_[i*2 + 0].fTime +
					posKeys_[i*2 + 1].fTime +
					posKeys_[i*2 + 2].fTime;
				new_pos_keys[i].fTime /= 3.0f;

				Vector3d v0 = posKeys_[i*2 + 0].vPos + (posKeys_[i*2 + 1].vPos - posKeys_[i*2 + 0].vPos) * 0.5f;
				Vector3d v1 = posKeys_[i*2 + 1].vPos + (posKeys_[i*2 + 2].vPos - posKeys_[i*2 + 1].vPos) * 0.5f;

				new_pos_keys[i].vPos = v0 + (v1 - v0) * 0.5f;
			}
			posKeys_.swap(new_pos_keys);
		}
	}


	void CSequence::destroy(void)
	{
		rotKeys_.clear();
		posKeys_.clear();
		
		rotKeyBuffer.clear();
		rotKeyBufferSize	= 0;
		rotKeyBufferCount	= 0;

		posKeyBuffer.clear();
		posKeyBufferSize	= 0;
		posKeyBufferCount	= 0;
	}


	float CSequence::getAnimationStartTime(void) const
	{
		if (posKeys_.empty() && rotKeys_.empty())
			return 0.0f;
		else
		{
			float fStartTime;
			if (!posKeys_.empty())
				fStartTime = posKeys_[0].fTime;
			else
				fStartTime = std::numeric_limits<float>::max();

			if (!rotKeys_.empty() && fStartTime > rotKeys_[0].fTime)
				fStartTime = rotKeys_[0].fTime;

			return fStartTime;
		}
	}

	float CSequence::getAnimationStopTime(void) const
	{
		if (posKeys_.empty() && rotKeys_.empty())
			return 0.0f;
		else
		{
			float fStopTime;
			if (!posKeys_.empty())
				fStopTime = posKeys_.back().fTime;
			else
				fStopTime = std::numeric_limits<float>::lowest();

			if (!rotKeys_.empty() && fStopTime < rotKeys_.back().fTime)
				fStopTime = rotKeys_.back().fTime;

			return fStopTime;
		}
	}


	bool CSequence::isContainedPosKeys(void) const
	{
		return (false == posKeys_.empty());
	}


	unsigned int CSequence::findInbetweenPosKeys(
		float fCurTime,
		const CPosKey*& pKeyFirst,
		const CPosKey*& pKeySecond,
		unsigned int nLastKeyIndex) const
	{
		const unsigned int nPosKeyCount = getPosKeyCount();
		if (0 == nPosKeyCount)
		{
			pKeyFirst = 0;
			pKeySecond = 0;
			return 0xffffffff;
		}
		else if (1 == nPosKeyCount)
		{
			pKeyFirst = &posKeys_[0];
			pKeySecond = &posKeys_[0];
			return 0;
		}
		else if (2 == nPosKeyCount)
		{
			pKeyFirst = &posKeys_[0];
			pKeySecond = &posKeys_[1];
			return 0;
		}
		else
		{
			if (fCurTime <= posKeys_[0].fTime)
			{
				pKeyFirst = &posKeys_[0];
				pKeySecond = &posKeys_[1];
				return 0;
			}
			else if (fCurTime >= posKeys_.back().fTime)
			{
				pKeyFirst = &posKeys_[nPosKeyCount - 2];
				pKeySecond = &posKeys_[nPosKeyCount - 1];
				//assert(nPosKeyCount >= 2);
				return nPosKeyCount - 2;
			}

			// Start to search
			unsigned int nKeyIndex = nLastKeyIndex;
			if (nKeyIndex >= static_cast<unsigned int>(posKeys_.size()))
				nKeyIndex = 0;

			for (unsigned int i = 0; i < nPosKeyCount; ++i)
			{
				nKeyIndex = (nKeyIndex + 1) % nPosKeyCount;
				if (posKeys_[nKeyIndex].fTime <= fCurTime &&
					nKeyIndex + 1 < nPosKeyCount &&
					fCurTime <= posKeys_[nKeyIndex + 1].fTime)
				{
					pKeyFirst = &posKeys_[nKeyIndex];
					pKeySecond = &posKeys_[nKeyIndex + 1];
					break;
				}
			}

			return nKeyIndex;
		}
	}


	unsigned int CSequence::findInbetweenRotKeys(
		float fCurTime,
		const CRotKey*& pKeyFirst,
		const CRotKey*& pKeySecond,
		unsigned int nLastKeyIndex) const
	{
		const unsigned int nRotKeyCount = getRotKeyCount();
		if (0 == nRotKeyCount)
		{
			pKeyFirst = 0;
			pKeySecond = 0;
			return 0xffffffff;
		}
		else if (1 == nRotKeyCount)
		{
			pKeyFirst = &rotKeys_[0];
			pKeySecond = &rotKeys_[0];
			return 0;
		}
		else if (2 == nRotKeyCount)
		{
			pKeyFirst = &rotKeys_[0];
			pKeySecond = &rotKeys_[1];
			return 0;
		}
		else
		{
			if (fCurTime <= rotKeys_[0].fTime)
			{
				pKeyFirst = &rotKeys_[0];
				pKeySecond = &rotKeys_[1];
				return 0;
			}
			else if (fCurTime >= rotKeys_.back().fTime)
			{
				pKeyFirst = &rotKeys_[nRotKeyCount - 2];
				pKeySecond = &rotKeys_[nRotKeyCount - 1];
				//assert(nRotKeyCount >= 2);
				return nRotKeyCount - 2;
			}

			// Start to search
			unsigned int nKeyIndex = nLastKeyIndex;
			if (nKeyIndex >= static_cast<unsigned int>(rotKeys_.size()))
				nKeyIndex = 0;

			for (unsigned int i = 0; i < nRotKeyCount; ++i)
			{
				nKeyIndex = (nKeyIndex + 1) % nRotKeyCount;
				if (rotKeys_[nKeyIndex].fTime <= fCurTime &&
					nKeyIndex + 1 < nRotKeyCount &&
					fCurTime < rotKeys_[nKeyIndex + 1].fTime)
				{
					pKeyFirst = &rotKeys_[nKeyIndex];
					pKeySecond = &rotKeys_[nKeyIndex + 1];
					break;
				}
			}

			return nKeyIndex;
		}
	}


	unsigned int CSequence::getPosKeyCount(void) const
	{
		return static_cast<unsigned int>(posKeys_.size());
	}


	unsigned int CSequence::getRotKeyCount(void) const
	{
		return static_cast<unsigned int>(rotKeys_.size());
	}


	bool CSequence::isContainedRotKeys(void) const
	{
		return (false == rotKeys_.empty());
	}
}