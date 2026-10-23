// Copyright Epic Games, Inc. All Rights Reserved.

#include "TAB_EEGameMode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TAB_EEGameMode)

ATAB_EEGameMode::ATAB_EEGameMode()
{

}

void ATAB_EEGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) // override
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Load the asset safely during InitGame instead of the constructor
    FSoftClassPath PawnClassPath(TEXT("/Game/Blueprints/SandboxCharacter_Mover_Ragdoll.SandboxCharacter_Mover_Ragdoll_C"));
    UClass* BPPawnClass = PawnClassPath.TryLoadClass<APawn>();
    if (BPPawnClass)
    {
        DefaultPawnClass = BPPawnClass;
    }

    FSoftClassPath PlayerControllerClassPath(TEXT("/Game/Blueprints/PC_Sandbox.PC_Sandbox_C"));
    UClass* BPPlayerControllerClass = PlayerControllerClassPath.TryLoadClass<APlayerController>();
    if (BPPlayerControllerClass)
    {
        PlayerControllerClass = BPPlayerControllerClass;
    }
}
