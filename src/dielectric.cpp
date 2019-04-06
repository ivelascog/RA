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

#include <nori/bsdf.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

/// Ideal dielectric BSDF
class Dielectric : public BSDF {
public:
    Dielectric(const PropertyList &propList) {
        /* Interior IOR (default: BK7 borosilicate optical glass) */
        m_intIOR = propList.getFloat("intIOR", 1.54f);

        /* Exterior IOR (default: air) */
        m_extIOR = propList.getFloat("extIOR", 1.000277f);
    }

	inline Vector3f refract(const Vector3f &wi, float cosThetaT,float eta) const {
		float scale = -(cosThetaT < 0 ? 1.0f/eta : eta);

		return Vector3f(scale * wi.x(), scale *wi.y(), cosThetaT);
	}

    Color3f eval(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return Color3f(0.0f);
    }

    float pdf(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return 0.0f;
    }

    Color3f sample(BSDFQueryRecord &bRec, const Point2f &sample) const {
		bRec.measure = EDiscrete;
		float cosThetaI = Frame::cosTheta(bRec.wi);
		float fresnelTerm =  fresnel(cosThetaI, m_extIOR, m_intIOR);

		if ( sample.x() <= fresnelTerm)
		{
			bRec.wo = Vector3f(
				-bRec.wi.x(),
				-bRec.wi.y(),
				bRec.wi.z()
			);
		} else
		{
			float eta = m_intIOR / m_extIOR;
			float scale = (cosThetaI > 0) ? 1 / eta : eta,
				cosThetaTSqr = 1 - (1 - cosThetaI * cosThetaI) * (scale*scale);
			float cosThetaT;
			/* Check for total internal reflection */
			if (cosThetaTSqr <= 0.0f) {
				cosThetaT = 0.0f;
			}

			/* Find the absolute cosines of the incident/transmitted rays */
			cosThetaT = std::sqrt(cosThetaTSqr);
			cosThetaT = (cosThetaI > 0) ? -cosThetaT : cosThetaT;
			bRec.wo = refract(bRec.wi, cosThetaT,eta);
		}

		return Color3f(0);
    }

    std::string toString() const {
        return tfm::format(
            "Dielectric[\n"
            "  intIOR = %f,\n"
            "  extIOR = %f\n"
            "]",
            m_intIOR, m_extIOR);
    }
private:
    float m_intIOR, m_extIOR;
};

NORI_REGISTER_CLASS(Dielectric, "dielectric");
NORI_NAMESPACE_END
