
#include "nori/common.h"
#include "nori/emitter.h"
#include "nori/bitmap.h"
#include <filesystem/path.h>
#include <filesystem/resolver.h>
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

	virtual Color3f eval(const EmitterQueryRecord & lRec) const {
		return m_texture(0,0);
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