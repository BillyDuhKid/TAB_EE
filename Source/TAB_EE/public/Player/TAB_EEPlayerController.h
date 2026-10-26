// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TAB_EEPlayerController.generated.h"

class UInputAction;

UCLASS()
class ATAB_EEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

	/** Cycle to the next visual override via the current game mode */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void NextVisualOverride();

};
