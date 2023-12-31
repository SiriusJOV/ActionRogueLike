// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;
class UDataTable;
class USMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	FPrimaryAssetId MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		// timer runs every 2 seconds then spawns bot at EQS query

protected:


	FString SlotName;

	UPROPERTY();
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "AI");
	UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI");
	UCurveFloat* DifficultyCurve;

	//UPROPERTY(EditDefaultsOnly, Category = "AI");
	//TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI");
	UEnvQuery* SpawnBotQuery;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI");
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		UEnvQuery* PowerupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<AActor>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float RequiredPowerupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 DesiredPowerupCount;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
		void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	virtual void StartPlay() override;

	ASGameModeBase();

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void WriteSaveGame();

	// Already managed in C++ so not marked as BP Callable. 
	void LoadSaveGame();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
};
