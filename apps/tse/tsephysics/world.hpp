#pragma once

#include <box3d/box3d.h>
#include <components/macros/class.hpp>

namespace TSE
{
    class World final
    {
    public:
        TSE_NON_COPYABLE_NON_MOVABLE(World)
        explicit World();
        ~World();

        b3WorldId getWorldId() const { return mWorldId; }

    private:
        b3WorldDef mWorldDef;
        b3WorldId mWorldId;
    };
}
