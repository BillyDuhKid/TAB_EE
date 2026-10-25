// Copyright Epic Games, Inc. All Rights Reserved.

#include "TAB_EEGameMode.h"

#include "HAL/IConsoleManager.h"

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


// WORKING here. is not being called but should be?
void ATAB_EEGameMode::CycleVisualOverride()
{
    UE_LOG(LogTemp, Warning, TEXT("BillyTest - CycleVisualOverride called"));

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
