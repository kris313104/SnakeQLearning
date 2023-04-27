// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Arena.generated.h"

/**
 *
 */
UCLASS()
class SNAKEGAMEQLEARNING_API AArena : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AArena();
	void BeginPlay();
	void GetRandomFruitSpawnLoc(FVector& OutLocation);

private:
	float WallWidth;
	float WallHeight;
	void SpawnWall(FVector SpawnLocation, FVector SpawnScale);


};
