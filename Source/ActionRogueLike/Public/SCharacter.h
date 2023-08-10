// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

// fwd decalres
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class UParticleSystem;
class USActionComponent;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects");
	FName TimeToHitParamName;


public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Attack");
	float AttackAnimDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USActionComponent* ActionComp;
	

	void MoveForward(float value);

	void SprintStart();

	void SprintStop();

	void MoveRight(float value);

	void PrimaryAttack();

	void PrimaryInteract();

	void BlackHoleAttack();

	void Dash();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec) // Turns into console command - only works in Game mode, Player Character, Character controller, Cheat Manager
	void HealSelf(float Amount=100);


};
