// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameplayTagContainer.h>
#include "SAction.generated.h"


//fwd declares
 
class UWorld;

// struct for isRunning
// NOTE: Inside a struct - we don't have to pass in REPLICATED. By default already replicated. 

USTRUCT()
struct FActionRepData 
{
	GENERATED_BODY();

public:

	UPROPERTY();
	bool bIsRunning;

	UPROPERTY();
	AActor* Instigator;

};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI");
	UTexture2D* Icon;

	UPROPERTY(Replicated);
	USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData");
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	float TimeStarted;


public:

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action");
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

	//UFUNCTION(BlueprintNativeEvent, Category = "Action")


	
	UPROPERTY(EditDefaultsOnly, Category = "Action");
	FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true; // implemented in header because it's a simple function
	}

};
