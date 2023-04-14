#include "Rendering.h"

vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;

IRenderingInterface::IRenderingInterface()
{
    create_guid(&Guid);
    RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
    for(vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();Iter!=RenderingInterface.end();++Iter)
    {
        if(*Iter == this)
        {
            RenderingInterface.erase(Iter);
            break;
        }
    }
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::Draw(float DeltaTime)
{

}
