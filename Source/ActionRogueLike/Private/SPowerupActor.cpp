// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");

	RootComponent = SphereComp;

	RespawnTime = 10.0f; // 10 seconds until respawns
	bIsActive = true;

	SetReplicates(true); // if server spawns an actor, it will tell all clients to also make a copy of it

}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();

	//RootComponent->SetVisibility(bNewIsActive, true);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

FText ASPowerupActor::GetInteractTest_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}

