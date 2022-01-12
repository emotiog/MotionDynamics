#include "define.h"
#include <GL/GLU.h>
#include "Vector.h"
#include "TransformMatrix.h"

namespace NAnimation
{
	bool InitOpenGL(void);

	extern void normalVec(CVector<double, 3>* _pNv, 
		const CVector<double, 3>& _rP1,
		const CVector<double, 3>& _rP2,
		const CVector<double, 3>& _rP3);

	extern void oglSphere(double _radius);

	extern void oglCylinder(double _height, double _radius);


	extern void drawLinkW(const CTransformMatrix& _rStart,
		const CTransformMatrix& _rEnd, double _radius);
}