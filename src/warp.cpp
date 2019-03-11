/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <nori/warp.h>
#include <nori/vector.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

Point2f Warp::squareToTent(const Point2f &sample) {
	float su0 = std::sqrt(sample.x());
	return Point2f(1 - su0, sample.y() * su0);
}

float Warp::squareToTentPdf(const Point2f &p) {
	if (p.y() >= 0 && p.x() >= 0 && p.x() + p.y() <= 1)
	{
		return 2;
	} else
	{
		return 0;
	}
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
	Point2f uOffset = 2.f * sample - Vector2f(1, 1);

	if (uOffset.x() == 0 && uOffset.y() == 0) return Point2f(0, 0);

	float theta, r;
	if (std::abs(uOffset.x()) > std::abs(uOffset.y()))
	{
		r = uOffset.x(); theta = M_PI / 4.0f * (uOffset.y() / uOffset.x());
	}
	else
	{
		r = uOffset.y(); theta = M_PI_2 - M_PI / 4.0f * (uOffset.x() / uOffset.y());
	} return r * Point2f(std::cos(theta), std::sin(theta));
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
	if (p.norm() <= 1) {
		return 1.0f / M_PI;
	} else
	{
		return 0.0f;
	}
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
	float z = 1 - 2 * sample.x();
	float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
	float phi = 2 * M_PI * sample.y();
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
	return 1.0f / (4.0f * M_PI);
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
	float teta = std::acos(sample.x());
	float phi = 2 * M_PI * sample.y();

	float x = std::sin(teta) * std::cos(phi);
	float y = std::sin(teta) * std::sin(phi);
	float z = std::cos(teta);

	return { x,y,z };
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
	if (v.z() >= 0) {
		return 1.0f / (2.0f * M_PI);
	} else
	{
		return 0.0f;
	}

}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
	Point2f d = squareToUniformDisk(sample);
	float z = std::sqrt(std::max(0.0f, 1.0f - d.x() * d.x() - d.y() * d.y()));
	return Vector3f(d.x(), d.y(), z);
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {

	
   if (v.z() >= 0)
   {
	   return v.z() / M_PI;
   } else
   {
	   return 0.0f;
   }
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    throw NoriException("Warp::squareToBeckmann() is not yet implemented!");
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    throw NoriException("Warp::squareToBeckmannPdf() is not yet implemented!");
}

NORI_NAMESPACE_END
