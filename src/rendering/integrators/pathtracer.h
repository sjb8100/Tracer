#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "vector.h"
#include "randomizer.h"
#include "ray.h"
#include "sphere.h"
#include "scenemanager.h"


class PathTracer
{
    public:
        PathTracer();

        Vector3 getRadiance(Ray& cameraRay,
            SceneManager& renderScene,
            Randomizer& randEngine,
            int rayDepth);

    private:

};


#endif // PATHTRACER_H
