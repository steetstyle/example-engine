#ifndef IGRAPHICSAPI_H
#define IGRAPHICSAPI_H

#include <GLFW/glfw3.h>

class IGraphicsAPI {
    public:
        virtual void Init(GLFWwindow* window) = 0;
        virtual void Update() = 0;
        virtual void Cleanup() = 0;

        virtual ~IGraphicsAPI() = default;
};

#endif // IGRAPHICSAPI_H