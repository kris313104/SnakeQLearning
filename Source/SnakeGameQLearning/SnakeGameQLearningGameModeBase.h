// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include <random>
#include "GameFramework/GameModeBase.h"
#include "SnakeGameQLearningGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SNAKEGAMEQLEARNING_API ASnakeGameQLearningGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASnakeGameQLearningGameModeBase();
	// void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void StartPlay() override;
	virtual void Tick(float DeltaTime) override;
	void GameOver();
	void SpawnArena();

	FVector StartingLocation;
	float ArenaWidth;
	float ArenaHeight;
	float CameraHeight;
	void SpawnFruit();

	// Initialize the Q-table with random values
	void InitializeQTable();
	// Select an action based on the current state
	int SelectAction();
	// Perform the selected action and observe the next state and the reward
	void PerformAction(int action);
	// Update the Q-table
	void UpdateQValue(int action);
	// Save the Q-table
	void SaveQTable(std::map<std::pair<int, int>, float> QTable);

	bool HitWallOrItself;

	void SetReward(bool _AteFruit);

private:
	float reward = 0;     // reward
	bool TailFront;
	bool TailLeft;
	bool TailRight;
	bool AteFruit;
	class AArena *Arena;
	int MAX_EPISODES;
	int MAX_STEPS;
};
