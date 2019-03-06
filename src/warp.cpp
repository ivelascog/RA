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
    throw NoriException("Warp::squareToTentPdf() is not yet implemented!");
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
	float r = std::sqrt(sample.x());
	float theta = 2.0f * M_PI * sample.y();
	return Point2f(r * std::cos(theta), r * std::sin(theta));
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
	return 0; //TODO
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
	float z = 1.0f - 2.0f * sample.x();
	float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
	float phi = 2.0f * M_PI * sample.y();
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
	return 1.0f / (4.0f * M_PI);
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
	float z = sample.x();
	float r = std::sqrt(std::max(0.0f, 1.0f - z * z)); 
	float phi = 2.0f * M_PI * sample.y();
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
	return 1.0f / (2.0f * M_PI);

}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
	Point2f d = squareToUniformDisk(sample);
	float z = std::sqrt(std::max(0.0f, 1.0f - d.x() * d.x() - d.y() * d.y()));
	return Vector3f(d.x(), d.y(), z);
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    throw NoriException("Warp::squareToCosineHemispherePdf() is not yet implemented!");
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    throw NoriException("Warp::squareToBeckmann() is not yet implemented!");
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    throw NoriException("Warp::squareToBeckmannPdf() is not yet implemented!");
}

NORI_NAMESPACE_END
