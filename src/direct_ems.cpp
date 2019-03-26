#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include "nori/warp.h"
#include "nori/emitter.h"
#include "nori/bsdf.h"

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
		float pdfDir = light->pdf(lightRecord);
		
		//BRDF Term
		BSDFQueryRecord bsdf_query{its.shFrame.toLocal(-ray.d),its.shFrame.toLocal(lightRecord.wi),ESolidAngle};

		Color3f brdf = its.mesh->getBSDF()->eval(bsdf_query);


		//Visibilty point sampled Light
		Ray3f shadowRay(xl, lightRecord.wi, 0.1f, INFINITY);
		scene->rayIntersect(shadowRay, its);
		bool V = its.mesh->getEmitter() == light;
		
		Color3f Li = V * light->eval(lightRecord);

		return Le + Li * brdf * nx.dot(-ray.d) / (pdfL * pdfDir);
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return "";
	}

protected:
};

NORI_REGISTER_CLASS(DirectEmitterSampling, "direct_ems");
NORI_NAMESPACE_END