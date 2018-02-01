#include <vector>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
    // TODO
    double min_t = std::numeric_limits<float>::max();
    Object * closest_object = NULL;
    std::vector<Hit> hitList;

    for (unsigned i = 0; i < objects.size(); i++)
    {
         
        objects.at(i)->Intersection(ray, hitList);
        for(unsigned j = 0; j < hitList.size(); j++)
        {
            if(hitList.at(j).t < min_t && hitList.at(j).t > small_t)
            {

                closest_object = objects.at(i);
                hit = hitList.at(j);
                min_t = hitList.at(j).t;
            }

        }
    }

    return closest_object;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    vec3 direction = camera.World_Position(pixel_index) - camera.position;

    Ray ray(camera.position, direction.normalized()); // TODO: set up the initial view ray here

    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO
    Hit intersection;
    Object* obj = Closest_Intersection(ray, intersection);
    vec3 color;
    vec3 dummy;

    if(recursion_depth > recursion_depth_limit)
    {
        color = background_shader->Shade_Surface(ray,dummy,dummy,1,false);
    }
    else if(obj != NULL)
    {

        vec3 intersectionPoint = ray.Point(intersection.t);
        vec3 normal = obj->Normal(intersectionPoint);

        if(intersection.ray_exiting)
        {
            normal = normal * -1;
        }

        color = obj->material_shader->Shade_Surface(ray,intersectionPoint,normal,1,intersection.ray_exiting);
    }
    else
    {
        color = background_shader->Shade_Surface(ray,dummy,dummy,1,false);
    }

    

    // determine the color here

    return color;
}
