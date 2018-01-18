#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
	// TODO
	double discriminant; 
	double t1;//solution 1
	double t2;//solution 2

	double a;
	double b;
	double c;
	Hit hit1;
	Hit hit2;


	a = dot(ray.direction, ray.direction);
	b = dot((ray.direction + ray.direction), (ray.endpoint - center));
	c = dot(ray.endpoint - center, ray.endpoint - center) - (radius*radius);
	discriminant = (b * b) - (4 * a * c);




	// std::cout<< "a: " << a << " ";
	// std::cout<< "b: " << b << " ";
	// std::cout<< "c: " << c << " ";
	// std::cout<< "d: " << discriminant << " ";

	if(discriminant <= 0)
	{
		
		return false;
	}
	else
	{
		t1 = ((b * -1) - discriminant) / (2 * a);
		t2 = ((b * -1) + discriminant) / (2 * a);

		if(t1 > small_t)
		{

			hit1.object = this;
			hit1.t = t1;
			hit1.ray_exiting = false;
		}

		if(t2 > small_t)
		{
			
			hit2.object = this;
			hit2.t = t2;
			hit2.ray_exiting = true;
		}

		hits.push_back(hit1);
		hits.push_back(hit2);
		return true;
	}
    
    return false;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    return normal;
}
