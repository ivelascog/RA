#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include "nori/warp.h"
#include "nori/emitter.h"
#include "nori/bsdf.h"

NORI_NAMESPACE_BEGIN

class PathTracing : public Integrator {
public:
	PathTracing(const PropertyList &props)
	{
		p_survival = props.getFloat("Psurvival", 0.8f);
		m_sampler = static_cast<Sampler*> (NoriObjectFactory::createInstance("independent", PropertyList()));
	}

	

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		Ray3f mRay = ray;
		Intersection its;
		Color3f L(0.0f);
		Color3f W = Color3f(1.0f);
		while (true) {

			if (!scene->rayIntersect(mRay, its)) {
				L += W * scene->getBackground(mRay);
				break;
			}
			
			Point3f xl = its.p;
			Normal3f nx = its.shFrame.n;
			Point2f sample = sampler->next2D();
			if (its.mesh->isEmitter())
			{
				EmitterQueryRecord lightRecord;
				auto emmiter = its.mesh->getEmitter();
				lightRecord.ref = xl;
				emmiter->sample(lightRecord, sample, 0.0f);
				L += W * emmiter->eval(lightRecord);
			}

			auto rand = m_sampler->next1D();
			if (rand <  p_survival)
			{
				break;
			}

			//BRDF sample
			BSDFQueryRecord bsdf_query_record(its.shFrame.toLocal(-mRay.d));
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

			 mRay = Ray3f(xl, its.shFrame.toWorld(bsdf_query_record.wo));
			if (!pdfDir == 0.0f){
				W *= brdf * abs(Frame::cosTheta(bsdf_query_record.wi)) /  (pdfDir * p_survival);
			}
		}
		return L;
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return "";
	}

protected:
	float p_survival;
	Sampler* m_sampler;
};

NORI_REGISTER_CLASS(PathTracing, "pathtracer");
NORI_NAMESPACE_END