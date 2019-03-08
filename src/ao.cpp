#include <nori/integrator.h>

NORI_NAMESPACE_BEGIN

class AoIntegrator : public Integrator {
public:
	AoIntegrator(const PropertyList &props)
	{
	}

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		return Color3f(0, 1, 0);
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return tfm::format(
			"AoIntegrator[\n"
			"  myProperty = \"%s\"\n"
			"]"
			
		);
	}

protected:
};

NORI_REGISTER_CLASS(AoIntegrator, "ao");
NORI_NAMESPACE_END