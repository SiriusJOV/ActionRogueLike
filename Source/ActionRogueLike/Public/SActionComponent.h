// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS(ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USActionComponent();

protected:

	UPROPERTY();
	TArray<USAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Actions");
	TArray<TSubclassOf<USAction>> DefaultActions;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		bool StopActionByName(AActor* Instigator, FName ActionName);

};
