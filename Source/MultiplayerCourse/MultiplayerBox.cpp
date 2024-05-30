// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerBox.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AMultiplayerBox::AMultiplayerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReplicatedVariable = 10;

}

// Called when the game starts or when spawned
void AMultiplayerBox::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	SetReplicatingMovement(true);
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMultiplayerBox::MulticastRPCExplode, 3, false);
	}
	
}

// Called every frame
void AMultiplayerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UGameplayStatics::GetPlayerPawn(this, 0) && HasAuthority())
	{
	Location = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation() + FVector(200, 0, 0);	
	}
}

void AMultiplayerBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerBox, ReplicatedVariable);
	DOREPLIFETIME(AMultiplayerBox, Location);
}

void AMultiplayerBox::OnRep_ReplicatedVariable()
{
	if (HasAuthority())
	{
		SetActorLocation(Location);
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Server"));
	}
	else
	{
		int32 PlayerID = GPlayInEditorID;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, 
		FString::Printf(TEXT("Client %d"), PlayerID));
	}	
}

void AMultiplayerBox::DecrementReplicatedVariable()
{
	if (HasAuthority())
	{
		ReplicatedVariable -= 1;
		OnRep_ReplicatedVariable();
		if (ReplicatedVariable > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMultiplayerBox::DecrementReplicatedVariable, 3, false);
		}
		
	}	
}

void AMultiplayerBox::MulticastRPCExplode_Implementation()
{
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Server: MulticastRPCFunction_Implementation"));
		GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMultiplayerBox::MulticastRPCExplode, 3, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Client: MulticastRPCFunction_Implementation"));
	}
	if (!IsRunningDedicatedServer())
	{
		if (ExplosionEffect)
		{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation() + FVector(0, 0, 200));
		}
	}
	
}


