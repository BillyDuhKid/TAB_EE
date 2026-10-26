// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/TAB_EEPlayerController.h"
#include "Game/TAB_EEGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "TAB_EE.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TAB_EEPlayerController)

//DEBUG: This maps IA_NextVisualOverride to NextVisualOverride() finding the correct IMC_Sandbox that has the binding in the editor. 
//TODO: Once merged with TAB_Game plugin use UTAB_InputActions for a better solution. Again this is unoptimized debug only functionality.
void ATAB_EEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			UInputMappingContext* SandboxMappingContext = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, TEXT("/Game/Input/IMC_Sandbox.IMC_Sandbox")));
			if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					if (SandboxMappingContext)
					{
						if (!InputSubsystem->HasMappingContext(SandboxMappingContext))
						{
							// IMC was not yet active — add it now so actions in it can fire.
							InputSubsystem->AddMappingContext(SandboxMappingContext, 0);
						}
					}
				}
			}

			if (UInputAction* InputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Input/IA_NextVisualOverride.IA_NextVisualOverride"))))
			{
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ATAB_EEPlayerController::NextVisualOverride);
			}
		}
	}
}

// DEBUG: cycles visual overrides. Something we'd likely never do in a production game.
void ATAB_EEPlayerController::NextVisualOverride()
{
	if (ATAB_EEGameMode* TAB_EEGameMode = Cast<ATAB_EEGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		TAB_EEGameMode->DEBUG_CycleVisualOverride();
	}
}
