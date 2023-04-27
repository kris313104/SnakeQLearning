// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZone.h"
#include "SnakeGameQLearningGameModeBase.h"

void ADeadZone::OverlapedWithSnakeHead(class ASnakeHead *SnakeHead)
{
    ASnakeGameQLearningGameModeBase* GameMode = Cast<ASnakeGameQLearningGameModeBase>(GetWorld()->GetAuthGameMode());
    if(GameMode)
    {
        GameMode->GameOver();
    }
}

