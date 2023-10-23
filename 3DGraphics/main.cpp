#include <bits\stdc++.h>

#include "BufferOutput.h"

using namespace std;

class Engine3D :public GameEngine
{
public:
    Engine3D();
    ~Engine3D();

    bool OnUserCreate() override
    {

        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {

        return true;
    }

private:

};

Engine3D::Engine3D()
{
}

Engine3D::~Engine3D()
{
}

int main() {
    Engine3D demo;
    demo.Build(420, 140);
    demo.Start();

    return 0;
}