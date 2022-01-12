#include "stdafx.h"
#include "openGL.h"
#include <gl/GL.h>
#include "Constants.h"

namespace NAnimation
{
	bool InitOpenGL(void)
	{
		glClearDepth(1.0f);
		glClearStencil(0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		return true;
	}


	void	normalVec(CVector<double, 3>* _pNv, 
		const CVector<double, 3>& _rP1,
		const CVector<double, 3>& _rP2,
		const CVector<double, 3>& _rP3)
	{
		CVector<double, 3> v1, v2, v3;

		v1	= _rP2 - _rP1;
		v2	= _rP3 - _rP1;
		
		v3	= v1.crossProduct(v2);

		double length	= v3.length();

		*_pNv	= v3/ length;
	}


	void	oglSphere(double _radius)
	{
		int slices = 15;
		GLUquadricObj* obj	= gluNewQuadric();

		gluSphere(obj, _radius, slices, slices);

		gluDeleteQuadric(obj);
	}


	void	oglCylinder(double _height, double _radius)
	{
		int slices	= 25;

		GLUquadricObj* obj	= gluNewQuadric();

		//glTranslated(0., 0., -_height);
		gluQuadricOrientation(obj, GLU_INSIDE);
		gluDisk(obj, 0., _radius, slices, 1);

		gluQuadricOrientation(obj, GLU_OUTSIDE);
		gluCylinder(obj, _radius, _radius, _height, slices, 1);

		glTranslated(0., 0., _height);
		gluDisk(obj, 0., _radius, slices, 1);

		//glTranslated(0., 0., -_height);

		gluDeleteQuadric(obj);
	}

	void drawLinkW(const CTransformMatrix& _rStart,
		const CTransformMatrix& _rEnd, double _radius)
	{
		CVector<double, 3> length;
		GLdouble	mStart[16]	= {
			_rStart.m11, _rStart.m12, _rStart.m13, _rStart.m14,
			_rStart.m21, _rStart.m22, _rStart.m23, _rStart.m24,
			_rStart.m31, _rStart.m32, _rStart.m33, _rStart.m34,
			_rStart.m41, _rStart.m42, _rStart.m43, _rStart.m44};

		length.x	= _rStart.m14 - _rEnd.m14;
		length.y	= _rStart.m24 - _rEnd.m24;
		length.z	= _rStart.m34 - _rEnd.m34;



		//double rz = (180. / 3.141592)* atan2(length.y, length.x);
		//double ry = 90. - (180. / 3.141592)* atan2(length.z , sqrt( length.x * length.x + length.y * length.y));
		double height	= sqrt( length.x * length.x + length.y * length.y + length.z * length.z);

		glMultMatrixd(mStart);
		//glRotated( rz , 0. , 0. , 1.);
		//glRotated( ry , 0. , 1. , 0.);
		//glTranslated(_rStart.x , _rStart.y , _rStart.z);

		oglCylinder( height , _radius);
	};
}