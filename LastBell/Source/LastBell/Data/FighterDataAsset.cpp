#include "Data/FighterDataAsset.h"

const FAttackData& UFighterDataAsset::GetAttackData(EBoxingMove Move) const
{
    switch (Move)
    {
        case EBoxingMove::Jab:      return JabData;
        case EBoxingMove::Hook:     return HookData;
        case EBoxingMove::Uppercut: return UppercutData;
        default:                    return JabData;
    }
}
