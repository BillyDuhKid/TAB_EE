// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TAB_MoverPawn.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultMovementSet/Modes/FlyingMode.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "Engine/World.h"
#include "MotionWarpingComponent.h"
#include "MoverComponent.h"
#include "PhysicsControlComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TAB_MoverPawn)

// Sets default values
ATAB_MoverPawn::ATAB_MoverPawn()
{
	// Create Component Structure
	// Note: UCapsuleComponent required for CharacterMover to work without crashing. EXCEPTION_ACCESS_VIOLATION
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleComponent);

	PhysicsControl = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControl"));
	PhysicsControl->SetupAttachment(SkeletalMesh);


	VisualOverride = CreateDefaultSubobject<UChildActorComponent>(TEXT("VisualOverride"));
	VisualOverride->SetupAttachment(SkeletalMesh);

	CharacterMover = CreateDefaultSubobject<UMoverComponent>(TEXT("CharacterMover"));
	CharacterMover->SetUpdatedComponent(CapsuleComponent);

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	NavMover = CreateDefaultSubobject<UNavMoverComponent>(TEXT("NavMover"));

	const TSoftClassPtr<UActorComponent> TraversalLogicClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/AC_TraversalLogic.AC_TraversalLogic"))
	);
	if (UClass* LogicClass = TraversalLogicClass.LoadSynchronous())
	{
		AC_TraversalLogic = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("AC_TraversalLogic"), UActorComponent::StaticClass(), LogicClass, true, false));
	}

	const TSoftClassPtr<UActorComponent> FoleyEventsClass(
		FSoftObjectPath(TEXT("/Game/Audio/Foley/AC_FoleyEvents.AC_FoleyEvents"))
	);
	if (UClass* FoleyClass = FoleyEventsClass.LoadSynchronous())
	{
		AC_FoleyEvents = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("AC_FoleyEvents"), UActorComponent::StaticClass(), FoleyClass, true, false));
	}

	const TSoftClassPtr<UBaseMovementMode> RagdollModeClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Ragdoll.BP_MovementMode_Ragdoll_C"))
	);
	if (UClass* RagdollClass = RagdollModeClass.LoadSynchronous())
	{
		CharacterMover->AddMovementModeFromClass(TEXT("Ragdoll"), RagdollClass);
	}

	const TSoftClassPtr<UActorComponent> VisualOverrideManagerClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/AC_VisualOverrideManager.AC_VisualOverrideManager_C"))
		);
	if (UClass* VOMClass = VisualOverrideManagerClass.LoadSynchronous())
	{
		VisualOverrideManager = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("VisualOverrideManager"), UActorComponent::StaticClass(), VOMClass, true, false));
	}

	// Configure Components
	if (CapsuleComponent)
	{
		CapsuleComponent->InitCapsuleSize(30.f, 86.f);
		CapsuleComponent->SetLineThickness(0.5f);
		CapsuleComponent->bReplicatePhysicsToAutonomousProxy = false;
		CapsuleComponent->CanCharacterStepUpOn = ECB_No;
		CapsuleComponent->SetCollisionProfileName(TEXT("CharacterCapsule"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BillyTest - ATAB_MoverPawn Failed to create CapsuleComponent!"));
	}

	if (VisualOverride)
	{
		VisualOverride->ComponentTags.AddUnique(TEXT("VisualOverride"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BillyTest - ATAB_MoverPawn Failed to create VisualOverride!"));
	}

	if (SkeletalMesh)
	{
		SkeletalMesh->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
		SkeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));//NOTE: BP was FRotator(0.f, 0.f, -90.f), but that doesn't appear to be right.
		SkeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin.SKM_UEFN_Mannequin")));
		SkeletalMesh->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Materials/MI_UEFN_Mannequin_Physics")));
		SkeletalMesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;
		SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
		SkeletalMesh->SetNotifyRigidBodyCollision(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BillyTest - ATAB_MoverPawn Failed to create SkeletalMesh!"));
	}

	if (PhysicsControl)
	{
		PhysicsControl->PhysicsControlAsset = LoadObject<UPhysicsControlAsset>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Rigs/PCA_SandboxCharacter.PCA_SandboxCharacter"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BillyTest - ATAB_MoverPawn Failed to create PhysicsControl!"));
	}

	if (CharacterMover)
	{
		//Walking
		const TSoftClassPtr<UBaseMovementMode> WalkingModeClass(
			FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Walking.BP_MovementMode_Walking_C"))
		);
		if (UClass* WalkingClass = WalkingModeClass.LoadSynchronous())
		{
			CharacterMover->AddMovementModeFromClass(TEXT("Walking"), WalkingClass);
		}
		//Falling
		const TSoftClassPtr<UBaseMovementMode> FallingModeClass(
			FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Falling.BP_MovementMode_Falling_C"))
		);
		if (UClass* FallingClass = FallingModeClass.LoadSynchronous())
		{
			CharacterMover->AddMovementModeFromClass(TEXT("Falling"), FallingClass);
		}
		//Slide
		const TSoftClassPtr<UBaseMovementMode> SlideModeClass(
			FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Slide.BP_MovementMode_Slide_C"))
		);
		if (UClass* SlideClass = SlideModeClass.LoadSynchronous())
		{
			CharacterMover->AddMovementModeFromClass(TEXT("Sliding"), SlideClass);
		}
		//Traversing
		CharacterMover->AddMovementModeFromClass(TEXT("Traversing"), UFlyingMode::StaticClass());
	}

	// Changes from BP defaults:
	SetReplicates(false);
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATAB_MoverPawn::BeginPlay()
{
	Super::BeginPlay();

	// Moved out of constructor was causing in-editor error viewing blueprints.
	// Assertion failed: Subsystem [File:D:\build++UE5\Sync\Engine\Source\Runtime\Engine\Classes\Animation\AnimInstance.h] [Line: 937]
	if (SkeletalMesh)
	{
		if (const UWorld* World = GetWorld(); World && World->IsGameWorld())
		{
			const TSoftClassPtr<UAnimInstance> AnimationModeClass(
				FSoftObjectPath(TEXT("/Game/Blueprints/SandboxCharacter_Mover_ABP.SandboxCharacter_Mover_ABP_C"))
			);
			if (UClass* ModeClass = AnimationModeClass.LoadSynchronous())
			{
				SkeletalMesh->SetAnimInstanceClass(ModeClass);
			}
		}
	}

	if (CharacterMover)
	{
		CharacterMover->OnPostFinalize.AddDynamic(this, &ATAB_MoverPawn::OnPostFinalize);
		CharacterMover->OnPreSimulationTick.AddDynamic(this, &ATAB_MoverPawn::OnPreSimulationTick);
		CharacterMover->OnBasedMovementApplied.AddDynamic(this, &ATAB_MoverPawn::OnBasedMovementApplied);
	}

	if (SkeletalMesh)
	{
		if (CharacterMover)
		{
			SkeletalMesh->AddTickPrerequisiteComponent(CharacterMover);
		}

		this->AddTickPrerequisiteComponent(SkeletalMesh);
	}

	MoverDefaultInputs_PreSim = this->GetActorForwardVector();
	InitialTransform = this->GetActorTransform();
}

void ATAB_MoverPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CharacterMover)
	{
		CharacterMover->OnPostFinalize.RemoveDynamic(this, &ATAB_MoverPawn::OnPostFinalize);
		CharacterMover->OnPreSimulationTick.RemoveDynamic(this, &ATAB_MoverPawn::OnPreSimulationTick);
		CharacterMover->OnBasedMovementApplied.RemoveDynamic(this, &ATAB_MoverPawn::OnBasedMovementApplied);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATAB_MoverPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATAB_MoverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATAB_MoverPawn::OnPostFinalize(const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState)
{
//	UE_LOG(LogTemp, Warning, TEXT("BillyTest - ATAB_MoverPawn::OnPostFinalize Called!"));
}

void ATAB_MoverPawn::OnPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd)
{

}

void ATAB_MoverPawn::OnBasedMovementApplied(const FTransform & TransformDelta, const FMoverTimeStep & TimeStep)
{

}
