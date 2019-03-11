#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include "nori/warp.h"
#include <limits>

NORI_NAMESPACE_BEGIN

class AoIntegrator : public Integrator {
public:
	AoIntegrator(const PropertyList &props)
	{
	}

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		Intersection its;
		if (!scene->rayIntersect(ray, its))
			return { 1, 1, 1 };
		sampler->generate();
		Vector3f d = Warp::squareToCosineHemisphere(sampler->next2D());
		Vector3f n = its.shFrame.n;
		d = its.shFrame.toWorld(d);

		const Ray3f aoRay(its.p, d, 0, std::numeric_limits<float>::max());

		return scene->rayIntersect(aoRay) ? Color3f{ 0, 0, 0 } : Color3f{ 1.0f, 1.0f, 1.0f } *n.dot(d);
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
    return "";
	}

protected:
};

NORI_REGISTER_CLASS(AoIntegrator, "ao");
NORI_NAMESPACE_END