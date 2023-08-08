// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ASAIController::BeginPlay() 
{
	Super::BeginPlay();


	if (ensureMsgf(BehaviorTree, TEXT("Behavior tree is null ptr - please assign BehaviorTree in AI controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// NO LONGER USED:
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn) {
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}



}