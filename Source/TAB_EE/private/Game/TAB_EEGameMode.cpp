// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/TAB_EEGameMode.h"
#include "Player/TAB_EEPlayerController.h"
#include "HAL/IConsoleManager.h"
#include "TAB_EE.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TAB_EEGameMode)

// This class replaces the GM_Sandbox blueprint game mode that is part of GameAnimationSample.
// Everything needs to work in C++ to interact with the current C++ plug-ins I'm using.
ATAB_EEGameMode::ATAB_EEGameMode()
	: Super()
{
    PlayerControllerClass = ATAB_EEPlayerController::StaticClass();

    // For testing these are the possible visual overrides we can cycle through.
    VisualOverrides_Soft.Add(TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("/Game/Blueprints/RetargetedCharacters/BP_Manny.BP_Manny_C"))));
    VisualOverrides_Soft.Add(TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("/Game/Blueprints/RetargetedCharacters/BP_Quinn.BP_Quinn_C"))));
    VisualOverrides_Soft.Add(TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("/Game/MetaHumans/Kellan/BP_Kellan.BP_Kellan_C"))));
}

// Override InitGame to safely load the asset references during game initialization for DefaultPawnClass.
// Blueprint classes can't be put in the constructor because they don't load properly can cause crashes if set in the constructor.
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
}

// DEBUG: Called From TAB_EEPlayerController: cycles visual overrides - built direction from the Blueprint unoptimized, unplanned to use in production.
// This override should likely only be used as an example so this entire game mode is likely example only.
void ATAB_EEGameMode::DEBUG_CycleVisualOverride()
{
    int32 PreviousIndex = 0;
    int32 NewIndex = 0;

    // Read current value of the console variable
    IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("DDCvar.VisualOverride"));
    if (CVar)
    {
        PreviousIndex = CVar->GetInt();
    }

    // Advance to the next index
    NewIndex = PreviousIndex + 1;

    // Wrap back to -1 (disabled) when we exceed the array bounds
    if (NewIndex >= VisualOverrides_Soft.Num())
    {
        NewIndex = -1;
    }

    // Execute the console command to apply the new index
    FString Command = FString::Printf(TEXT("DDCvar.VisualOverride  %d"), NewIndex);
    if (UWorld* World = GetWorld())
    {
        GEngine->Exec(World, *Command);
    }
}
