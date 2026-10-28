// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TAB_MoverPawn.h"


//Add "Mover" to PublicDependencyModuleNames.AddRange in build.cs.
//#include "Backends/MoverNetworkPredictionLiaison.h"
#include "MoverComponent.h"
#include "DefaultMovementSet/Modes/FlyingMode.h"
//#include "DefaultMovementSet/Modes/SmoothWalkingMode.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ATAB_MoverPawn::ATAB_MoverPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Note: UCapsuleComponent required for CharacterMover to work without crashing. EXCEPTION_ACCESS_VIOLATION
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(42.f, 96.f);
	SetRootComponent(CapsuleComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleComponent);
	SkeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin.SKM_UEFN_Mannequin")));
	SkeletalMesh->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Characters/UEFN_Mannequin/Materials/MI_UEFN_Mannequin_Physics")));


	// Don't want to troubleshoot visual overrides now, but this appears to correctly set the class
	//const TSoftClassPtr<UActorComponent> VisualOverrideManagerClass(
	//	FSoftObjectPath(TEXT("/Game/Blueprints/AC_VisualOverrideManager.AC_VisualOverrideManager_C"))
	//);
	//if (UClass* VOMClass = VisualOverrideManagerClass.LoadSynchronous())
	//{
	//	VisualOverrideManager = Cast<UActorComponent>(CreateDefaultSubobject(TEXT("VisualOverrideManager"), UActorComponent::StaticClass(), VOMClass, true, false));
	//}

	CharacterMover = CreateDefaultSubobject<UMoverComponent>(TEXT("CharacterMover"));
	CharacterMover->SetUpdatedComponent(CapsuleComponent);
	// Movement Modes: Walking, Falling, Sliding, Traversing, Ragdoll
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
	
	//Ragdoll
		const TSoftClassPtr<UBaseMovementMode> RagdollModeClass(
			FSoftObjectPath(TEXT("/Game/Blueprints/MovementModes/BP_MovementMode_Ragdoll.BP_MovementMode_Ragdoll_C"))
		);
		if (UClass* RagdollClass = RagdollModeClass.LoadSynchronous())
		{
			CharacterMover->AddMovementModeFromClass(TEXT("Ragdoll"), RagdollClass);
		}
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

void ATAB_MoverPawn::OnPostFinalize(const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState)
{

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

