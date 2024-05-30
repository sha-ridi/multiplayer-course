// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerBox.generated.h"

UCLASS()
class MULTIPLAYERCOURSE_API AMultiplayerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMultiplayerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedVariable, BlueprintReadWrite)
	float ReplicatedVariable;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void OnRep_ReplicatedVariable();

	UPROPERTY(Replicated)
	FVector Location;

	FTimerHandle TestTimer;
	
	void DecrementReplicatedVariable();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastRPCExplode();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionEffect;
};
