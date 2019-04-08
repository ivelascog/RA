#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include "nori/warp.h"
#include "nori/emitter.h"
#include "nori/bsdf.h"

NORI_NAMESPACE_BEGIN

class DirectMaterialSampling : public Integrator {
public:
	DirectMaterialSampling(const PropertyList &props)
	{
	}

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		Intersection its;
		if (!scene->rayIntersect(ray, its))
			return { 0 };

		Point3f xl = its.p;
		Normal3f nx = its.shFrame.n;
		Color3f Le(0.0f);
		Point2f sample = sampler->next2D();
		if (its.mesh->isEmitter())
		{
			EmitterQueryRecord lightRecord;
			auto emmiter = its.mesh->getEmitter();
			lightRecord.wi = ray.d;
			lightRecord.dist = 0.1f;
			Le = emmiter->eval(lightRecord);
		}

		//BRDF sample
		BSDFQueryRecord bsdf_query_record(its.shFrame.toLocal(-ray.d));
		its.mesh->getBSDF()->sample(bsdf_query_record, sample);
		Color3f brdf;
		float pdfDir;
		if (bsdf_query_record.measure == EDiscrete) {
			brdf = Color3f(1.0f);
			pdfDir = 1.0f;
		}
		else {
			brdf = its.mesh->getBSDF()->eval(bsdf_query_record);
			pdfDir = its.mesh->getBSDF()->pdf(bsdf_query_record);
		}

		Ray3f brdfRay = Ray3f(xl, its.shFrame.toWorld(bsdf_query_record.wo));

		
		Color3f Li(0.0f);
		if (!scene->rayIntersect(brdfRay,its))
		{
			Li = scene->getBackground(brdfRay);
		}
		else {
			EmitterQueryRecord emitter_query_record;
			if (its.mesh->isEmitter())
			{
				auto light = its.mesh->getEmitter();

				emitter_query_record.ref = xl;
				emitter_query_record.emitter = light;
				emitter_query_record.p = its.p;
				emitter_query_record.n = its.shFrame.n;
				Vector3f refToP = its.p - xl;
				emitter_query_record.dist = refToP.norm();
				emitter_query_record.wi = refToP.normalized();
				emitter_query_record.pdf = 1.0f;
				Li = light->eval(emitter_query_record);
			}
		}

		if (pdfDir == 0.0f)
		{
			return Le;
		}
		else {
			return Le + Li * brdf * Frame::cosTheta(bsdf_query_record.wi) / pdfDir;
		}
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return "";
	}

protected:
};

NORI_REGISTER_CLASS(DirectMaterialSampling, "direct_mats");
NORI_NAMESPACE_END