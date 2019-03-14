#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include "nori/warp.h"
#include "nori/emitter.h"

NORI_NAMESPACE_BEGIN

	class DirectEmitterSampling : public Integrator {
public:
	DirectEmitterSampling (const PropertyList &props)
	{
	}

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		Intersection its;
		if (!scene->rayIntersect(ray, its))
			return { 0 };

		Point3f xl = its.p;
		Normal3f nx = its.shFrame.n;
		Color3f Le (0.0f);
		Point2f sample = sampler->next2D();
		if (its.mesh->isEmitter())
		{
			EmitterQueryRecord lightRecord;
			auto emmiter = its.mesh->getEmitter();
			lightRecord.ref = xl;
			emmiter->sample(lightRecord, sample, 0.0f);
			Le = emmiter->eval(lightRecord);
		}

		//sample Light
		float pdfL;
		auto light = scene->sampleEmitter(sample.x(), pdfL);
		EmitterQueryRecord lightRecord;
		lightRecord.ref = xl;
		light->sample(lightRecord, sample, 0.0f);

		//Sample dir
		Vector3f dir = Warp::squareToCosineHemisphere(sample);
		float pdfDir = Warp::squareToCosineHemispherePdf(dir);

		//Visibilty point sampled Light
		Ray3f shadowRay(xl, lightRecord.wi,0.0f,INFINITY );
		scene->rayIntersect(shadowRay, its);
		bool V = its.mesh->getEmitter() == light;

		float brdf = 1.0f;

		Color3f Li = V * light->eval(lightRecord);

		return Le + Li * brdf * nx.dot(dir) / (pdfL * pdfDir);
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return "";
	}

protected:
};

NORI_REGISTER_CLASS(DirectEmitterSampling, "direct_ems");
NORI_NAMESPACE_END