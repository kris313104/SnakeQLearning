// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Overlapable.generated.h"

UCLASS()
class SNAKEGAMEQLEARNING_API AOverlapable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOverlapable();

	UPROPERTY()
	USceneComponent* RootComp;
	UPROPERTY()
	UStaticMeshComponent* Mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void OverlapedWithSnakeHead(class ASnakeHead* SnakeHead);
};
