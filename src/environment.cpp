
#include "nori/common.h"
#include "nori/emitter.h"
#include "nori/bitmap.h"
#include <filesystem/path.h>
#include <filesystem/resolver.h>
#include <algorithm>
#include <cmath>
NORI_NAMESPACE_BEGIN

class Enviorement: public Emitter {
public:
	Enviorement(const PropertyList &props) {
		m_type = EmitterType::EMITTER_ENVIRONMENT;
		m_filename = getFileResolver()->resolve(props.getString("filename"));
		m_texture = Bitmap(m_filename.str());
	}

	virtual std::string toString() const {
		return tfm::format(
			"EnviorementMap[\n"
			"  filename = %s,\n"
			"]",
			m_filename.str());
	}

	inline float clamp(float v, float min, float max) const
	{
		return std::max(min, std::min(v, max));
	}

	virtual Color3f eval(const EmitterQueryRecord & lRec) const {
		Vector3f w = lRec.wi.normalized();
		Point2f uv(
			(std::atan2(w.x(), -w.z())) * INV_TWOPI,
			std::acos(clamp(w.y(),-1,1)) * INV_PI
		);
		int x = uv.x() * (m_texture.cols() - 1);
		x = (int)x % m_texture.cols();
		x = x >= 0 ? x : x + m_texture.cols();
		int y = uv.y() * (m_texture.rows() - 1);
		y = clamp(y, 0, m_texture.rows() - 1);
		return m_texture(y, x);
	}

	virtual Color3f sample(EmitterQueryRecord & lRec, const Point2f & sample, float optional_u) const {
		throw NoriException("Not implemented");
	}

	
	virtual float pdf(const EmitterQueryRecord &lRec) const {
		throw NoriException("Not implemented");
	}
private:
	filesystem::path m_filename;
	Bitmap m_texture;
};


NORI_REGISTER_CLASS(Enviorement, "enviorement")
NORI_NAMESPACE_END