// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "TAB_MoverPawn.generated.h"

class UActorComponent;
class UMoverComponent;
class UCapsuleComponent;
class UNavMoverComponent;
class USkeletalMeshComponent;
class UMotionWarpingComponent;
class UPhysicsControlComponent;

UCLASS()
class TAB_EE_API ATAB_MoverPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATAB_MoverPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//TODO: Cleanup as many UPROPERTY() as possible.
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UChildActorComponent* VisualOverride;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UActorComponent* VisualOverrideManager;

	// Declare the Physics Control Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UPhysicsControlComponent* PhysicsControl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UMoverComponent* CharacterMover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UNavMoverComponent* NavMover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UActorComponent* AC_TraversalLogic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UActorComponent* AC_FoleyEvents;



	// Fired after a frame has been finalized. This may be a resimulation or not. No changes to state are possible. Guaranteed to be on the game thread.
	// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMover_OnPostFinalize, const FMoverSyncState&, SyncState, const FMoverAuxStateContext&, AuxState);
	UFUNCTION()
	void OnPostFinalize(const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState);

};
