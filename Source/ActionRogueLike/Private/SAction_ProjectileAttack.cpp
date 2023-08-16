// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
		if (ensureAlways(ProjectileClass))
		{
			FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = InstigatorCharacter;

			FHitResult Hit;
			FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

			// endpoint of look at distance
			FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);


			FCollisionShape Shape;
			Shape.SetSphere(20.0f);

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(InstigatorCharacter);

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
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		}

		StopAction(InstigatorCharacter);

}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if (Character->HasAuthority())
		{
			FTimerHandle Timerhandle_AttackDelay;

			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(Timerhandle_AttackDelay, Delegate, AttackAnimDelay, false);

		}

	}


}



