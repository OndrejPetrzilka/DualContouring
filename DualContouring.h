// DualContouring.h
#include "ImplicitIsosurface.h"
#include "threed\isomesher_dc.h"
#include "threed\mesh.h"
#include "box.h"
#include <math.h>
#include "InterfaceIsosurface.h"
#include "Face.h"

#pragma once

using namespace ThreeD;
using namespace SharpDX;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

private delegate double DensityDelegate(float x, float y, float z);

namespace DualContouring 
{
	typedef double (*DensityFunctionPointerType)(float, float, float);
	
	public ref class IsoMesher
	{
	private:
		array<unsigned char, 3>^ voxels;
		DensityDelegate^ deleg;
		DensityFunctionPointerType delegatePointer;

		int x, y, z;
		float voxelSize;

	public:
		IsoMesher(int x, int y, int z, float voxelSize, array<unsigned char, 3>^ voxels)
		{
			this->voxels = voxels;
			this->x = x;
			this->y = y;
			this->z = z;
			this->voxelSize = voxelSize;

			deleg = gcnew DensityDelegate(this, &IsoMesher::density);
			delegatePointer = (DensityFunctionPointerType) Marshal::GetFunctionPointerForDelegate(deleg).ToPointer();
		}

		static List<Face^>^ Run(Isosurface* surf, float voxelSize, float voxelScale)
		{
			//Isosurface *box2Iso = new BoxIsosurface(Vector(1.0f, 1.0f, 1.0f));
			////Isosurface *box2Iso = new BoxIsosurface(Vector(0.5f, 0.5f, 0.5f));
			//Transform box2Trans;
			//box2Trans.translate(Vector(-2.0f, -2.0f, 0.0f));
			//box2Trans.rotate(Vector(0.45f, 0.45f, 0.45f));
			//box2Iso->setTransform(Transform());

			ThreeD::IsoMesher_DC mesher(surf);
			//mesher.setVoxelSize(0.5f, 0.5f, 0.55f);
			mesher.setVoxelSize(voxelSize * voxelScale, voxelSize * voxelScale, voxelSize * voxelScale);
			ThreeD::Mesh *mesh = mesher.createMesh();
			mesh->computeVertexNormals();

			List<Face^>^ result = gcnew List<Face^>(1024);

			int i;
			Mesh::PlanesList::const_iterator itPlanes = mesh->getPlanes()->begin();
			while (itPlanes != mesh->getPlanes()->end()) 
			{
				const Mesh::FacesList &faces = (*itPlanes).faces;
				Mesh::FacesList::const_iterator itFaces = faces.begin();
				while (itFaces != faces.end()) 
				{
					auto face = gcnew Face((*itFaces)->_v[0][0], (*itFaces)->_v[1][0], (*itFaces)->_v[2][0], (*itFaces)->getNormal(0),  (*itFaces)->getNormal(1),  (*itFaces)->getNormal(2));
					result->Add(face);
					++itFaces;
				}
				++itPlanes;
			}
			return result;
		}

		static List<Face^>^ Calculate(int x, int y, int z, float voxelSize, float voxelScale, IIsosurface^ surface)
		{
			ThreeD::Vector bounds(x * voxelSize, y * voxelSize, z * voxelSize);
			InterfaceIsosurface surf(ThreeD::Vector(0,0,0), bounds, surface);
			return Run(&surf, voxelSize, voxelScale);
		}

		List<Face^>^ Calculate(float voxelScale)
		{
			/*
			ThreeD::Vector length = ThreeD::Vector(0.5f, 0.5f, 0.55f) / 2.0f;
			ThreeD::Vector _center = ThreeD::Vector();
			ThreeD::Vector _length2 = length * length;
			ThreeD::Vector corner1 = _center - length;
			ThreeD::Vector corner2 = _center + length;
			ThreeD::Vector bounds(x * voxelSize, y * voxelSize, z * voxelSize);
			ImplicitIsosurface isosurface(corner1, corner2, delegatePointer);
			ThreeD::IsoMesher_DC mesher(&isosurface);
			mesher.setVoxelSize(0.5f, 0.5f, 0.55f);
			*/

			ThreeD::Vector bounds(x * voxelSize, y * voxelSize, z * voxelSize);
			ImplicitIsosurface isosurface(ThreeD::Vector(0,0,0), bounds, delegatePointer);
			return Run(&isosurface, voxelSize, voxelScale);
		}

	private:
		float pow(float x)
		{
			return x * x;
		}

		float dd(float xt, float yt, float zt, float& coef)
		{			
			float off = 0.5f;
			int ix = (int)floor(xt / voxelSize + off);
			int iy = (int)floor(yt / voxelSize + off);
			int iz = (int)floor(zt / voxelSize + off);

			float dens = den(ix, iy, iz);
			if(dens < 127 || ix >= this->x || iy >= this->y || iz >= this->z || ix < 0 || iy < 0 || iz < 0)
			{
				coef = 0;
				return 0;
			}

			xt -= ix * voxelSize;
			yt -= iy * voxelSize;
			zt -= iz * voxelSize;

			float d2 = pow(xt * xt) + pow(yt * yt) + pow(zt * zt);
			float maxDist2 = pow(voxelSize);

			/*d2 = sqrt(d2);
			maxDist2 = sqrt(maxDist2);*/

			// TODO: make sqrt to make is linear
			float d = (maxDist2 - d2) / maxDist2;
			coef = d > 0 ? d : 0;
			return d > 0 ? d * dens : 0;
		}

		double den(int ix, int iy, int iz)
		{
			if(ix >= this->x || iy >= this->y || iz >= this->z || ix < 0 || iy < 0 || iz < 0)
				return voxelSize;

			return voxels[ix, iy, iz];
		}

		double density(float xt, float yt, float zt)
		{
			//float cc;
			////return 127 - dd(xt, yt, zt, cc);

			/*float bx = xt - voxelSize / 2;
			float by = yt - voxelSize / 2;
			float bz = zt - voxelSize / 2;

			float val;
			float coef = 0;
			float c;

			for(int x = 0; x <= 1; x++)
			{
				for(int y = 0; y <= 1; y++)
				{
					for(int z = 0; z <= 1; z++)
					{
						val += dd(bx + x * voxelSize, by + y * voxelSize, bz + z * voxelSize, c);
						coef += c;
					}
				}
			}

			if(coef <= 0)
				return 127;

			return 127 - (val / coef);*/

			//ThreeD::Vector length = ThreeD::Vector(0.5f, 0.5f, 0.55f) / 2.0f;
			//ThreeD::Vector _center = ThreeD::Vector();
			//ThreeD::Vector _length2 = length * length;

			///*xt -= _center.x();
			//yt -= _center.y();
			//zt -= _center.z();*/

			//ThreeD::Vector length2(3,4,6);

			//double xd = (xt * xt) - _length2.x();
			//double yd = (yt * yt) - _length2.y();
			//double zd = (zt * zt) - _length2.z();
			//double d = THREED_MAX(zd, THREED_MAX(xd, yd));

			//return d;

			//return 0.0f;

			float off = 0.5f;
			//off = 0.0f;
			int ix = (int)floor(xt / voxelSize + off);
			int iy = (int)floor(yt / voxelSize + off);
			int iz = (int)floor(zt / voxelSize + off);

			if(ix >= this->x || iy >= this->y || iz >= this->z)
				return voxelSize;

			// When full, calc square distance to center
			float den = voxels[ix, iy, iz];
			//if(den > 127)
			{
				float len2 = voxelSize * voxelSize;
				xt -= ix * voxelSize;
				yt -= iy * voxelSize;
				zt -= iz * voxelSize;

				double ds = xt * xt + yt * yt + zt * zt - len2;
				return ds * den;
				//return ds < 0 ? -den : den;

				double xd = (xt * xt) - len2;
				double yd = (yt * yt) - len2;
				double zd = (zt * zt) - len2;
				double d = THREED_MAX(zd, THREED_MAX(xd, yd));

				if(den > 127)
					return d * den;
				else
					return 127 - d * den;

				if(d < 0)
					return d * den / 255.0;
				else if(d > 0)
					return d * den / 255.0;
				else 
					return 0;

				return d * (den - 127) / 255;
			}
			//return 0.001f;
		}

		float isFull(int ix, int iy, int iz)
		{
			if(ix >= this->x || iy >= this->y || iz >= this->z || ix < 0 || iy < 0 || iz < 0)
				return 0;

			return voxels[ix, iy, iz] > 127 ? 1.0f : 0.0f;
		}

		float denSurr(int ix, int iy, int iz)
		{
			float sum = 0;
			for(int x = -1; x <= 1; x++)
			{
				for(int y = -1; y <= 1; y++)
				{
					for(int z = -1; z <= 1; z++)
					{
						sum += isFull(ix + x, iy + y, iz + z);
					}
				}
			}
			return sum / 9;
		}
	};
}
