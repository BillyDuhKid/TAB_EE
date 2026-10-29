// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TAB_MoverPawn.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/Modes/FlyingMode.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "MotionWarpingComponent.h"
#include "MoverComponent.h"
#include "PhysicsControlComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TAB_MoverPawn)

// Sets default values
ATAB_MoverPawn::ATAB_MoverPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Changes from BP defaults:
	SetReplicates(false);
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Note: UCapsuleComponent required for CharacterMover to work without crashing. EXCEPTION_ACCESS_VIOLATION
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(30.f, 86.f);
	CapsuleComponent->SetLineThickness(0.5f);
	CapsuleComponent->bReplicatePhysicsToAutonomousProxy = false;
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetCollisionProfileName(TEXT("CharacterCapsule"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleComponent);
	SkeletalMesh->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	SkeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));//NOTE: BP was FRotator(0.f, 0.f, -90.f), but that doesn't appear to be right.

	const TSoftClassPtr<UAnimInstance> AnimationModeClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/SandboxCharacter_Mover_ABP.SandboxCharacter_Mover_ABP_C"))
	);
	if (UClass* ModeClass = AnimationModeClass.LoadSynchronous())
	{
		SkeletalMesh->SetAnimInstanceClass(ModeClass);
	}

	SkeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin.SKM_UEFN_Mannequin")));
	SkeletalMesh->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Materials/MI_UEFN_Mannequin_Physics")));
	SkeletalMesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;
	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	SkeletalMesh->SetNotifyRigidBodyCollision(true);
	
	PhysicsControl = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControl"));
	PhysicsControl->SetupAttachment(SkeletalMesh);
	PhysicsControl->PhysicsControlAsset = LoadObject<UPhysicsControlAsset>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Rigs/PCA_SandboxCharacter.PCA_SandboxCharacter"));

	// Don't want to troubleshoot visual overrides now, but this appears to correctly set the class
	//const TSoftClassPtr<UActorComponent> VisualOverrideManagerClass(
	//	FSoftObjectPath(TEXT("/Game/Blueprints/AC_VisualOverrideManager.AC_VisualOverrideManager_C"))
	//);
	//if (UClass* VOMClass = VisualOverrideManagerClass.LoadSynchronous())
	//{
	//	VisualOverrideManager = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("VisualOverrideManager"), UActorComponent::StaticClass(), VOMClass, true, false));
	//}
	// Need to add Component Tags "VisualOverride"
	// NOTE: BP had a VisualOverride under the SkeletalMesh.

// Movement
	// Movement Modes: Walking, Falling, Sliding, Traversing, Ragdoll
	CharacterMover = CreateDefaultSubobject<UMoverComponent>(TEXT("CharacterMover"));
	CharacterMover->SetUpdatedComponent(CapsuleComponent);
	if (CharacterMover)
	{
	//Walking
		//TSoftClassPtr<UBaseMovementMode> WalkingModeClass(
		//	FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Walking.BP_MovementMode_Walking_C"))
		//);

		//UClass* LoadedClass = WalkingModeClass.LoadSynchronous();
		//if (LoadedClass)
		//{
		//	UBaseMovementMode* WalkingMode = LoadedClass->GetDefaultObject<UBaseMovementMode>();
		//	CharacterMover->AddMovementModeFromObject(TEXT("Walking"), WalkingMode);
		//} // This is old remove if everything below works.
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

	const TSoftClassPtr<UActorComponent> TraversalLogicClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/AC_TraversalLogic.AC_TraversalLogic"))
	);
	if (UClass* LogicClass = TraversalLogicClass.LoadSynchronous())
	{
		AC_TraversalLogic = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("AC_TraversalLogic"), UActorComponent::StaticClass(), LogicClass, true, false));
	}

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	NavMover = CreateDefaultSubobject<UNavMoverComponent>(TEXT("NavMover"));

// Sound
	const TSoftClassPtr<UActorComponent> FoleyEventsClass(
		FSoftObjectPath(TEXT("/Game/Audio/Foley/AC_FoleyEvents.AC_FoleyEvents"))
	);
	if (UClass* FoleyClass = FoleyEventsClass.LoadSynchronous())
	{
		AC_FoleyEvents = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("AC_FoleyEvents"), UActorComponent::StaticClass(), FoleyClass, true, false));
	}

//Ragdoll
	const TSoftClassPtr<UBaseMovementMode> RagdollModeClass(
		FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Ragdoll.BP_MovementMode_Ragdoll_C"))
	);
	if (UClass* RagdollClass = RagdollModeClass.LoadSynchronous())
	{
		CharacterMover->AddMovementModeFromClass(TEXT("Ragdoll"), RagdollClass);
	}
}

// Called when the game starts or when spawned
void ATAB_MoverPawn::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterMover)
	{
		CharacterMover->OnPostFinalize.AddDynamic(this, &ATAB_MoverPawn::OnPostFinalize);
	}
}

void ATAB_MoverPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CharacterMover)
	{
		CharacterMover->OnPostFinalize.RemoveDynamic(this, &ATAB_MoverPawn::OnPostFinalize);
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
