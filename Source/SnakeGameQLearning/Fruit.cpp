// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"
#include "SnakeHead.h"
#include "SnakeGameQLearningGameModeBase.h"
AFruit::AFruit()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if(MeshForMesh.Succeeded())
	{
		Mesh->SetStaticMesh(MeshForMesh.Object);
		Mesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5));


	}
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialForMesh(TEXT("/Game/StarterContent/Materials/M_Wood_Walnut"));
	if(MaterialForMesh.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialForMesh.Object);
	}
}

void AFruit::OverlapedWithSnakeHead(class ASnakeHead* SnakeHead)
{
    SnakeHead->EatFruit();
	ASnakeGameQLearningGameModeBase* GameMode = Cast<ASnakeGameQLearningGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->SpawnFruit();
		GameMode->SetReward(true);
	}
	Destroy();
}



