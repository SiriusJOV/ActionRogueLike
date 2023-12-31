// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0;

	SetReplicates(true); // COnsiders this actor an updates clients whenever marked as 'interesting'

}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	// When function is called, lid will be rotated


	bLidOpened = !bLidOpened;
	OnRep_LidOpened();


}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened(); // Ensures visual state is updated each time saved and loaded
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}


void ASItemChest::OnRep_LidOpened() 
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));


}