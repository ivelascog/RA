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
		m_depth = props.getInteger("depth", 5);
		m_sampler = static_cast<Sampler*> (NoriObjectFactory::createInstance("independent", PropertyList()));
	}

	

	/// Compute the radiance value for a given ray. Just return green here
	Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
		Ray3f mRay = ray;
		Intersection its;
		int bounces = 0;
		Color3f L(0.0f);
		Color3f W = Color3f(1.0f);
		while (bounces <= m_depth) {
			bounces++;
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
				lightRecord.ref = mRay.o;
				lightRecord.p = xl;
				lightRecord.n = nx;
				lightRecord.dist = its.t;
				lightRecord.wi = (lightRecord.p - lightRecord.ref).normalized();
				L += W * emmiter->eval(lightRecord);
			}

			//BRDF sample
			BSDFQueryRecord bsdf_query_record(its.shFrame.toLocal(-mRay.d));
			Color3f brdf = its.mesh->getBSDF()->sample(bsdf_query_record, sample);
			
			 mRay = Ray3f(xl, its.shFrame.toWorld(bsdf_query_record.wo));
			 W *= brdf;
		}
		return L;
	}

	/// Return a human-readable description for debugging purposes
	std::string toString() const {
		return "";
	}

protected:
	int m_depth;
	Sampler* m_sampler;
};

NORI_REGISTER_CLASS(PathTracing, "pathtracer");
NORI_NAMESPACE_END