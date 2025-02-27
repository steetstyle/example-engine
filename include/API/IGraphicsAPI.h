#ifndef IGRAPHICSAPI_H
#define IGRAPHICSAPI_H

class IGraphicsAPI {
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Cleanup() = 0;
};

#endif // IGRAPHICSAPI_H