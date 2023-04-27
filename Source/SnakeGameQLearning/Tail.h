// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadZone.h"
#include "Tail.generated.h"

/**
 *
 */
UCLASS()
class SNAKEGAMEQLEARNING_API ATail : public ADeadZone
{
	GENERATED_BODY()

public:
	ATail();
	void BeginPlay() override;
};
