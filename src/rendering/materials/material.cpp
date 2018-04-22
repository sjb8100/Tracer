#include "math_helper.h"

#include "material.h"


BSDF::BSDF()
{

}


void BSDF::materialSetup()
{
    fresnelR0 = std::max(((fresnelColor.x + fresnelColor.y + fresnelColor.z) / 3.0f), 0.04f);
}


const Vector3 BSDF::computeSampling(Vector3& rayDirection,
    const Vector3& hitNormal,
    Randomizer& randEngine)
{
    Vector3 computedColor;

    if (hitNormal.dot(rayDirection) < 0.0f)
    {
        Vector3 reflectVector = computeSchlick(rayDirection, hitNormal);

        // Specular
        if (Vector3(randEngine.getRandomFloat()) < reflectVector)
        {
            Vector3 reflectedRay = coneSampling(computeReflectedRay(rayDirection, hitNormal), roughness, randEngine);

            if (hitNormal.dot(reflectedRay) > 0.0f)
            {
                rayDirection = reflectedRay;
                computedColor = Vector3(1.0f).lerp(fresnelColor, roughness);

                return computedColor;
            }
            else
            {
                rayDirection =  hemisphereSampling(hitNormal, randEngine);

                // Lambertian diffuse
                return color * (1.0f / M_PI);
            }
        }

        else if (randEngine.getRandomFloat() < metalness)
            return Vector3();

        // Diffuse
        else
        {
            rayDirection =  hemisphereSampling(hitNormal, randEngine);

            // Lambertian diffuse
            return color * (1.0f / M_PI);
        }
    }
    else
    {

    }

    return computedColor;
}


const Vector3 BSDF::hemisphereSampling(const Vector3& rayDirection,
    Randomizer& randEngine)
{
    const float rand1 = randEngine.getRandomFloat();
    const float rand2 = randEngine.getRandomFloat();
    const float r = std::sqrt(rand1);
    const float theta = 2.0f * M_PI * rand2;
    const float x = r * std::cos(theta);
    const float y = r * std::sin(theta);

    Vector3 u = rayDirection.cross(sphereRandomSampling(randEngine.getRandomFloat() * M_PI * 2.0f,
        std::asin(randEngine.getRandomFloat() * 2.0f - 1.0f))).normalize();
    Vector3 v = rayDirection.cross(u);

    Vector3 newRayDir = rayDirection * (std::sqrt(1 - rand1));
    newRayDir += (u * x);
    newRayDir += (v * y);

    return newRayDir.normalize();
}


const Vector3 BSDF::coneSampling(const Vector3& rayDirection,
    float factor,
    Randomizer& randEngine)
{
    const float rand1 = randEngine.getRandomFloat();
    const float rand2 = randEngine.getRandomFloat();
    const float phi = factor * 0.5f * M_PI * (1.0f - (2.0f * std::acos(rand1) / M_PI));
    const float theta = 2.0f * M_PI * rand2;
    const float x = std::sin(phi);
    const float y = std::cos(phi);

    Vector3 u = rayDirection.cross(sphereRandomSampling(randEngine.getRandomFloat() * M_PI * 2.0f,
        std::asin(randEngine.getRandomFloat() * 2.0f - 1.0f)));
    Vector3 v = rayDirection.cross(v);

    Vector3 newRayDir = (rayDirection * y);
    newRayDir += (u * (x * std::cos(theta)));
    newRayDir += (v * (x * std::sin(theta)));

    return newRayDir.normalize();
}


const Vector3 BSDF::sphereRandomSampling(float theta,
    float phi)
{
    return Vector3(std::cos(theta) * std::cos(phi), std::sin(phi), std::sin(theta) * std::cos(phi));
}


const Vector3 BSDF::computeSchlick(const Vector3& rayDirection,
    const Vector3& hitNormal)
{
    float LdotH = -hitNormal.dot(rayDirection);

    return fresnelColor + ((std::max(Vector3(1.0f - roughness), fresnelColor) - fresnelColor) * std::pow(1.0f - LdotH, 5.0f));
    //return fresnelColor + ((Vector3(1.0f) - fresnelColor) * std::pow(1.0f - LdotH, 5.0f));
}


const Vector3 BSDF::computeReflectedRay(const Vector3& rayDirection,
    const Vector3& hitNormal)
{
    float cos = hitNormal.dot(rayDirection);

    return (rayDirection - (hitNormal * (2.0f * cos))).normalize();
}
