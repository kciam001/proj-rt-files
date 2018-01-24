#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
 	double solution = dot(normal, x1 - ray.endpoint) / dot(normal, ray.direction);
	Hit hit;

	if(dot(normal, ray.direction) != 0)
	{
		if( solution > 0)
		{
			hit.object = this;
			hit.t = solution;
			hit.ray_exiting = true;
			hits.push_back(hit);
			return true;
		}
	}

	return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
