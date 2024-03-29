// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MovementSpline.h"
#include "Player/PlayerControllerBase.h"
#include "Player/Projectile.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Camera boom");
	SpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->bDoCollisionTest = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
	PlayerCamera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Character mesh");
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Collision sphere");
	CollisionSphere->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, SOCKETS_CENTER);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECC_Pawn);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::HandleOverlap);

	RollingMovementSound = CreateDefaultSubobject<UAudioComponent>("Movement sound");
	RollingMovementSound->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMusicComp = CreateDefaultSubobject<UAudioComponent>("Background music");
	BackgroundMusicComp->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlayerPawn::AddTearFluid(int32 AddedAmount)
{
	CurrentScore += AddedAmount;
	if (CurrentTearFluid == MaximumTearFluid)
	{
		return;
	}

	CurrentTearFluid = FMath::Min(MaximumTearFluid, CurrentTearFluid + AddedAmount);
	OnTearFluidAmountChanged.Broadcast();
	PlaySound(EPlayerPawnSoundType::PPST_FillTear);
}

void APlayerPawn::RemoveTearFluidAmount(int32 RemovedAmount)
{
	CurrentTearFluid -= RemovedAmount;
	if (CurrentTearFluid <= 0)
	{
		CurrentTearFluid = 0;
		HandleGameOver();
	}

	OnTearFluidAmountChanged.Broadcast();
}

void APlayerPawn::ReceiveDamage(int32 DamageAmount)
{
	CurrentScore -= DamageAmount;
	RemoveTearFluidAmount(DamageAmount);
	PlaySound(EPlayerPawnSoundType::PPST_TakeDamage);
}

// ReSharper disable once CppMemberFunctionMayBeConst - bound to overlap delegate
void APlayerPawn::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ATriggerBox>())
	{
		HandleGameOver();
	}
}

void APlayerPawn::MoveLeftRight(float AxisValue)
{
	MovementInput.Y = AxisValue * MaxMovementSpeed;
}

void APlayerPawn::MoveForwardBackward(float AxisValue)
{
	MovementInput.X = AxisValue * MaxMovementSpeed;
}

void APlayerPawn::Shoot()
{
	if (ensure(ProjectileClass) && ProjectileClass.GetDefaultObject()->GetTearFluidCost() <= CurrentTearFluid)
	{
		if (const APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());
			ensure(PlayerController))
		{
			FVector TargetLocation = PlayerController->GetCursorWorldLocation();

			// don't allow shooting back
			if (TargetLocation.X < Mesh->GetComponentLocation().X)
			{
				TargetLocation = Mesh->GetComponentLocation();
			}

			AProjectile::SpawnProjectile(GetWorld(), ProjectileClass, this, this, Mesh->GetComponentLocation(),
			                             TargetLocation);
			RemoveTearFluidAmount(ProjectileClass.GetDefaultObject()->GetTearFluidCost());

			PlayAnimation(AnimationData.ShootAnimation);
		}
	}
}

void APlayerPawn::PlayAnimation(UAnimSequence* Animation, float PlaybackSpeed)
{
	if (ensure(Animation))
	{
		Mesh->PlayAnimation(Animation, false);
		Mesh->SetPlayRate(PlaybackSpeed);
		SetRestoreMovementTimer(Animation->GetPlayLength());
	}
}

void APlayerPawn::SetRestoreMovementTimer(float Delay)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(RestoreMovementTimer) < Delay)
	{
		GetWorld()->GetTimerManager().SetTimer(RestoreMovementTimer, this, &APlayerPawn::RestoreMovement, Delay);
	}
}

void APlayerPawn::HandleGameEnd()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()); ensure(PlayerController))
	{
		PlayerController->InputComponent->ClearActionBindings();
		PlayerController->InputComponent->ClearAxisBindings();
		if (ensure(HighScoreScreenClass))
		{
			UUserWidget* HighScoreScreen = CreateWidget(PlayerController, HighScoreScreenClass);
			HighScoreScreen->AddToViewport();
		}
	}

	RollingMovementSound->Stop();
	BackgroundMusicComp->Stop();
	SetActorTickEnabled(false);
}

void APlayerPawn::HandleGameOver()
{
	RollingMovementSound->Stop();
	BackgroundMusicComp->Stop();
	if (bIsGameOver)
	{
		return;
	}

	TArray<AActor*> MovementSplines;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMovementSpline::StaticClass(), MovementSplines);

	if (ensure(MovementSplines.Num() == 1))
	{
		MovementSplines.Last()->SetActorTickEnabled(false);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()); ensure(PlayerController))
	{
		PlayerController->InputComponent->ClearActionBindings();
		PlayerController->InputComponent->ClearAxisBindings();
		if (ensure(GameOverScreenClass))
		{
			UUserWidget* GameOverScreen = CreateWidget(PlayerController, GameOverScreenClass);
			GameOverScreen->AddToViewport();
		}
	}

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsGameOver = true;
}

FVector APlayerPawn::GetImpulse() const
{
	return (Mesh->GetComponentLocation() - PreviousTickLocation) * Mass;
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(INPUT_MOVEMENT_LEFT_RIGHT, this, &APlayerPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis(INPUT_MOVEMENT_FWD_BWD, this, &APlayerPawn::MoveForwardBackward);
	PlayerInputComponent->BindAction(INPUT_SHOOT, EInputEvent::IE_Pressed, this, &APlayerPawn::Shoot);
	PlayerInputComponent->BindAction(INPUT_JUMP, EInputEvent::IE_Released, this, &APlayerPawn::StartJump);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> MovementSplines;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMovementSpline::StaticClass(), MovementSplines);

	if (ensure(MovementSplines.Num() == 1))
	{
		Cast<AMovementSpline>(MovementSplines.Last())->ControlPawn(this);
	}

	PreviousTickLocation = Mesh->GetComponentLocation();

	RestoreMovement();
}

void APlayerPawn::PlaySound(EPlayerPawnSoundType SoundType) const
{
	if (const TArray<TObjectPtr<USoundWave>>* SelectedSounds = SoundData.GetSoundsByType(SoundType))
	{
		const TArray<TObjectPtr<USoundWave>>& Sounds = *SelectedSounds;

		if (Sounds.IsEmpty())
		{
			return;
		}

		const int32 SoundIndex = FMath::RandRange(0, Sounds.Num() - 1);;
		if (USoundWave* Sound = Sounds[SoundIndex])
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Mesh->GetComponentLocation(),
			                                      Mesh->GetComponentRotation());
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mesh->AddRelativeLocation(MovementInput * DeltaTime);

	TearDecayIntervalTime += DeltaTime;
	if (TearDecayIntervalTime > TearFluidDecay)
	{
		TearDecayIntervalTime -= TearFluidDecay;
		RemoveTearFluidAmount(1);
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(RestoreMovementTimer))
	{
		Mesh->SetPlayRate((Mesh->GetComponentLocation().X - PreviousTickLocation.X) * DeltaTime);
	}

	PreviousTickLocation = Mesh->GetComponentLocation();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerPawn::RestoreMovement()
{
	if (ensure(AnimationData.RollLoop))
	{
		Mesh->PlayAnimation(AnimationData.RollLoop, true);
	}

	PlaySound(EPlayerPawnSoundType::PPST_JumpEnd);
	RollingMovementSound->SetPaused(false);
}
