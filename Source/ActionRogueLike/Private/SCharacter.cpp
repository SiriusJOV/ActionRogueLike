// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	AttackAnimDelay = 0.2f;

	TimeToHitParamName = "TimeToHit";
	HandSocketName = "Muzzle_01";
}


void ASCharacter::MoveForward(float value) {

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value) {

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Axis allows to move forwards and backwards
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Jump

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); // This is a built in function which allows turning
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}

void ASCharacter::HealSelf(float Amount /*=100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PostInitializeComponents() {

	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta) {
	
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC); // Prevents moving around
	}
}


void ASCharacter::PrimaryAttack() {

	StartAttackEffects();

	// After 2 seconds the timer will trigger the PrimaryAttack function Spawning the projectile
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);


}

void ASCharacter::PrimaryAttack_TimeElapsed() {

	SpawnProjectile(ProjectileClass);
}

void ASCharacter::PrimaryInteract() {

	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
	
}

// Adding in functions
void ASCharacter::BlackHoleAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay);
}



void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);

}



void ASCharacter::Dash()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);
}


void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

//void ASCharacter::PrimaryAttack_TimeElapsed() {
//
//	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
//
//	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
//
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnParams.Instigator = this;
//
//	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
//}

void ASCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();

		// endpoint of look at distance
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);


		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// if we get a blocking hit it returns true

		FRotator ProjRotation;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}

		//Find direction and rotation from hand pointing to impact point in world


		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);

	}
}


