// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameplayTagContainer.h>
#include "SAction.generated.h"


//fwd declares
 
class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer BlockedTags;


	bool bIsRunning;


public:

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

};
