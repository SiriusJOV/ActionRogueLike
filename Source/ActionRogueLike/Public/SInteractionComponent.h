// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();

	UPROPERTY();
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	USWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
