#ifndef USDSCENE_H
#define USDSCENE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <cstring>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/base/vt/array.h>

#include "vector.h"
#include "sphere.h"
#include "mesh.h"
#include "material.h"
#include "render_helper.h"



class USDScene
{
    public:
        USDScene();

        int loadSceneFile(const std::string& scenePath);

        void loadMaterials(std::vector<BSDF> &materialsList);
        void loadSpheres(std::vector<Sphere> &spheresList, std::vector<BSDF>& materialsList);
        void loadMeshes(std::vector<Mesh> &meshesList, std::vector<BSDF>& materialsList);
        void loadLights(std::vector<Sphere> &spheresList, std::vector<BSDF>& materialsList);
        void loadCamera(cameraData &sceneCamera);
        void loadSettings(settingsData &sceneSettings);

        void getPrimFromType(const std::string &primType, const pxr::SdfPath &primPath, std::vector<pxr::UsdPrim>& primVector);

    private:
        pxr::UsdStageRefPtr sceneStage;
};


#endif // USDSCENE_H
