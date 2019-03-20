/*
	This file is part of Nori, a simple educational ray tracer
	Copyright (c) 2015 by Romain Prévost
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

#include <nori/emitter.h>
#include <nori/warp.h>
#include <nori/mesh.h>

NORI_NAMESPACE_BEGIN

class AreaEmitter : public Emitter {
public:
	AreaEmitter(const PropertyList &props) {
		m_type = EmitterType::EMITTER_AREA;
		m_radiance = props.getColor("radiance");
	}

	virtual std::string toString() const {
		return tfm::format(
			"AreaLight[\n"
			"  radiance = %s,\n"
			"]",
			m_radiance.toString());
	}

	// We don't assume anything about the visibility of points specified in 'ref' and 'p' in the EmitterQueryRecord.
	virtual Color3f eval(const EmitterQueryRecord & lRec) const {
		if (!m_mesh)
			throw NoriException("There is no shape attached to this Area light!");

		// This function call can be done by bsdf sampling routines.
		// Hence the ray was already traced for us - i.e a visibility test was already performed.
		// Hence just check if the associated normal in emitter query record and incoming direction are not backfacing
		if (lRec.wi.dot(lRec.n) > 0) { //EL rayo no pasa por la hemiesfera del triangulo.
			return 0.0f;
		} else
		{
			return m_radiance / std::pow(lRec.dist,2);
		}

	}

	virtual Color3f sample(EmitterQueryRecord & lRec, const Point2f & sample, float optional_u) const {
		if (!m_mesh)
			throw NoriException("There is no shape attached to this Area light!");
		Point3f p;
		Normal3f n;
		m_mesh->samplePosition(sample, p, n);
		lRec.pdf = m_mesh->pdf(p);
		lRec.n = n.normalized();
		lRec.p = p;
		Vector3f refToP = lRec.p - lRec.ref;
		lRec.dist = refToP.norm();
		lRec.wi = (lRec.p - lRec.ref).normalized();
		return 0.0f; //TODO  Ni idea de que se supone que tiene que devolver esto.
	}

	// Returns probability with respect to solid angle given by all the information inside the emitterqueryrecord.
	// Assumes all information about the intersection point is already provided inside.
	// WARNING: Use with care. Malformed EmitterQueryRecords can result in undefined behavior. Plus no visibility is considered.
	virtual float pdf(const EmitterQueryRecord &lRec) const {
		if (!m_mesh)
			throw NoriException("There is no shape attached to this Area light!");

		//Area to solid-angle
		float solidAnglePdf = lRec.pdf  * (lRec.dist / abs(lRec.n.dot(lRec.wi)));

		return solidAnglePdf;
	}


	// Get the parent mesh
	void setParent(NoriObject *parent)
	{
		auto type = parent->getClassType();
		if (type == EMesh)
			m_mesh = static_cast<Mesh*>(parent);
	}


protected:
	Color3f m_radiance;
};

NORI_REGISTER_CLASS(AreaEmitter, "area")
NORI_NAMESPACE_END
