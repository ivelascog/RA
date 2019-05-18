
#include "nori/common.h"
#include "nori/emitter.h"
#include "nori/bitmap.h"
#include <filesystem/path.h>
#include <filesystem/resolver.h>
#include <algorithm>
NORI_NAMESPACE_BEGIN
static const float InvPi = 0.31830988618379067154; static const float Inv2Pi = 0.15915494309189533577;
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
		Vector3f w = lRec.wi;
		Point2f uv(
			(atan2(w.x(), -w.z())) * INV_TWOPI,
			std::acos(clamp(w.y(),-1,1)) * INV_PI
		);
		uv[0] = std::max(uv[0], 0.0f);
		return m_texture(uv.y() * (m_texture.rows() - 1), uv.x() * (m_texture.cols() - 1));
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