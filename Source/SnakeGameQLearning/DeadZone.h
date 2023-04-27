// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Overlapable.h"
#include "DeadZone.generated.h"

/**
 *
 */
UCLASS()
class SNAKEGAMEQLEARNING_API ADeadZone : public AOverlapable
{
	GENERATED_BODY()

public:
	void OverlapedWithSnakeHead(class ASnakeHead *SnakeHead) override;
};
