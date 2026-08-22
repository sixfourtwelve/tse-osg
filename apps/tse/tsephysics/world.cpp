#include "world.hpp"

namespace TSE
{
    World::World()
    {
        mWorldDef = b3DefaultWorldDef();
        mWorldDef.gravity = (b3Vec3){ 0.0f, -10.0f, 0.0f };
        mWorldId = b3CreateWorld(&mWorldDef);
    }

    World::~World()
    {
        b3DestroyWorld(mWorldId);
    }
}
