// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SnakeHead.generated.h"


UCLASS()
class SNAKEGAMEQLEARNING_API ASnakeHead : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	ASnakeHead();
	UPROPERTY()
	USceneComponent *RootComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *HeadMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EatFruit();

	void RemoveTails();

	void Move();
	FVector MoveDir;
	bool DirLeft;
	bool DirRight;
	bool DirUp;
	bool DirDown;
	void MoveForward();
	void MoveToLeft();
	void MoveToRight();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	float GetMoveStepSize();

private:

	float MoveStepSize;
	int Score;
	TArray<class ATail *> Tails;
};
