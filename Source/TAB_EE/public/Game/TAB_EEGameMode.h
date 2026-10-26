// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TAB_EEGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS()
class ATAB_EEGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Constructor */
	ATAB_EEGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Cycles the visual override CVar to the next index in VisualOverrides_Soft, wrapping back to -1 when the end is reached. */
	UFUNCTION(BlueprintCallable, Category = "Visual Override")
	void CycleVisualOverride();

	/** Array of soft actor class references representing available visual overrides. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Override")
	TArray<TSoftClassPtr<AActor>> VisualOverrides_Soft;

};
