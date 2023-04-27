// Copyright Epic Games, Inc. All Rights Reserved.

#include "SnakeGameQLearningGameModeBase.h"
#include "SnakeHead.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Fruit.h"
#include "Arena.h"
#include "Tail.h"
#include "iostream"
#include "fstream"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);
std::map<std::pair<int, int>, float> Q; // Q-table

const int numStates = 13;          // number of states
const int numActions = 3;          // number of actions
const int numDirections = 4;       // number of directions
const float Alpha = 0.05;           // learning rate
const float Gamma = 0.9;           // discount factor
float epsilon = 0.3;              // exploration rate
const float foodReward = 1000;      // reward for eating food
const float gameOverReward = -1000; // reward for game over
const float moveReward = 1;       // reward for moving to a tile without food
//  QValue = QValue + Alpha * (reward + Gamma * maxQValue - QValue);
int steps = 0;  //
int Episode = 1;      // current episode
int currentState = 0; // current state
int nextState = 0;    // next state


ASnakeGameQLearningGameModeBase::ASnakeGameQLearningGameModeBase()
{
    DefaultPawnClass = ASnakeHead::StaticClass();
    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1;
    ArenaHeight = 2000;
    ArenaWidth = 2000;
    CameraHeight = 2000;
    StartingLocation = FVector(0, 0, 10);
    reward = 0;
    HitWallOrItself = false;
    MAX_EPISODES = 1000;
    MAX_STEPS = 400;
    InitializeQTable();



}

// void ASnakeGameQLearningGameModeBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
//     // Super::SetupPlayerInputComponent(PlayerInputComponent);
//     // ensure(PlayerInputComponent);
//     PlayerInputComponent->BindAction("SaveQTable", IE_Pressed, this, &ASnakeGameQLearningGameModeBase::SaveQTable);
// }

void ASnakeGameQLearningGameModeBase::StartPlay()
{

    Super::StartPlay();
    FVector SpawnLoc(0, 0, CameraHeight);
    FRotator SpawnRotation(-90, 0, 0);
    ACameraActor *PlayerViewCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLoc, SpawnRotation, FActorSpawnParameters());
    APlayerController *PC = UGameplayStatics::GetPlayerController(this, 0);

    if (PC)
    {
        PlayerViewCamera->SetOwner(PC);
        PC->SetViewTarget(PlayerViewCamera);
    }
    SpawnArena();
    SpawnFruit();

    // UE_LOG(LogTemp, Warning, TEXT("Episode number: %i"), Episode);
}
void ASnakeGameQLearningGameModeBase::Tick(float DeltaTime)
{
    //? QLearning
    if(Episode >= 700)
        epsilon = 0.05;

    int action = SelectAction(); // Select an action based on the current state
    PerformAction(action); // Perform the selected action and observe the next state and the reward
    UpdateQValue(action);
    SaveQTable(Q);
    if (steps != MAX_STEPS)
    {
        steps++;

        // UpdateQValue(action);


        if (reward == gameOverReward)
        {
            HitWallOrItself = false;
        }
        else {
            // Update the Q-value for the current state-action pair
            currentState = nextState;
        }
        steps++;
        UE_LOG(LogTemp, Warning, TEXT("CurrentState: %i, Reward: %f"),currentState, reward);
    }
    else
    {
        // steps = 0;
        // GameOver();
        HitWallOrItself = false;
        TArray<AActor *> Fruits;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFruit::StaticClass(), Fruits);

    for (AActor *f : Fruits)
        f->Destroy();

    TArray<AActor *> Tails;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATail::StaticClass(), Tails);

    for (AActor *t : Tails)
        t->Destroy();

    TArray<AActor *> Head;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeHead::StaticClass(), Head);

    for (AActor *h : Head)
    {
        ASnakeHead *SnakeHead = Cast<ASnakeHead>(h);
        SnakeHead->SetActorLocation(StartingLocation);
        SnakeHead->RemoveTails();
        SnakeHead->MoveDir = FVector(0, 0, 0);
    }

    Episode++;
    // UE_LOG(LogTemp, Warning, TEXT("Game number: %i"), Episode);
    SaveQTable(Q);
    steps = 0;
    SpawnFruit();
    }

}
void ASnakeGameQLearningGameModeBase::GameOver()
{
    HitWallOrItself = true;
    TArray<AActor *> Fruits;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFruit::StaticClass(), Fruits);

    for (AActor *f : Fruits)
        f->Destroy();

    TArray<AActor *> Tails;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATail::StaticClass(), Tails);

    for (AActor *t : Tails)
        t->Destroy();

    TArray<AActor *> Head;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeHead::StaticClass(), Head);

    for (AActor *h : Head)
    {
        ASnakeHead *SnakeHead = Cast<ASnakeHead>(h);
        SnakeHead->SetActorLocation(StartingLocation);
        SnakeHead->RemoveTails();
        SnakeHead->MoveDir = FVector(0, 0, 0);
    }

    Episode++;
    // UE_LOG(LogTemp, Warning, TEXT("Game number: %i"), Episode);
    SaveQTable(Q);
    steps = 0;
    SpawnFruit();
}

void ASnakeGameQLearningGameModeBase::SpawnArena()
{
    FTransform SpawnTransform = FTransform(FRotator(), FVector(), FVector(ArenaHeight, ArenaWidth, 1));
    Arena = GetWorld()->SpawnActor<AArena>(AArena::StaticClass(), SpawnTransform, FActorSpawnParameters());
}

void ASnakeGameQLearningGameModeBase::SpawnFruit()
{
    FVector SpawnLocation;
    Arena->GetRandomFruitSpawnLoc(SpawnLocation);
    GetWorld()->SpawnActor<AFruit>(AFruit::StaticClass(), SpawnLocation, FRotator(), FActorSpawnParameters());
}

void ASnakeGameQLearningGameModeBase::InitializeQTable()
{
    for (int i = 1; i <= numStates; i++)
    {
        for (int j = 1; j <= numActions; j++)
        {
            std::pair<int, int> stateActionPair = std::make_pair(i, j);
            float randomValue = dis(gen);

            //! initialize w/ 1
            Q[stateActionPair] = 1;

            //! initialize w/ random value
            // Q[stateActionPair] = randomValue;


        }
    }
}

int ASnakeGameQLearningGameModeBase::SelectAction()
{
    std::random_device rdd;
    std::mt19937 genn(rdd());
    std::uniform_real_distribution<> diss(0, 1);
    float randomValue = diss(genn);

    // with probability epsilon, select a random action
    if (randomValue < epsilon) {
        std::uniform_int_distribution<> action_dis(0, numActions - 1);
        return action_dis(genn);
    }
    else {
        // with probability 1-epsilon, select the action with the highest Q-value
        int bestAction = 0;
        float bestQValue = -1000000;
        for (int action = 0; action < numActions; action++) {
            std::pair<int, int> stateActionPair = std::make_pair(currentState, action);
            float QValue = Q[stateActionPair];
            if (QValue > bestQValue) {
                bestQValue = QValue;
                bestAction = action;
            }
        }
        return bestAction;
    }
}


// Update the Q-value for the current state-action pair
void ASnakeGameQLearningGameModeBase::UpdateQValue(int action) {
    std::pair<int, int> stateActionPair = std::make_pair(currentState, action);
    float QValue = Q[stateActionPair];

    // Q-Learning update rule
    float maxQValue = -1000000;
    for (int a = 1; a <= numActions; a++) {
        std::pair<int, int> nextStateActionPair = std::make_pair(nextState, a);
        maxQValue = std::max(maxQValue, Q[nextStateActionPair]);
    }
    QValue = QValue + Alpha * (reward + Gamma * maxQValue - QValue);

    Q[stateActionPair] = QValue;
}

void ASnakeGameQLearningGameModeBase::PerformAction(int action)
{
    TailFront = false;
	TailLeft = false;
	TailRight = false;

    TArray<AActor *> Head1;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeHead::StaticClass(), Head1);

    TArray<AActor *> Fruits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFruit::StaticClass(), Fruits);



    FVector FruitLocation;

    for (AActor *f : Fruits)
    {
        FruitLocation = f->GetActorLocation();
    }
    // code to move the snake based on the action
    //!action number:      1           2          3
    //!actions:      (go forward, turn right, turn left)
    for (AActor *h : Head1)
    {

        ASnakeHead *SnakeHead = Cast<ASnakeHead>(h);
        if(action == 1) SnakeHead->MoveForward();

        if(action == 2) SnakeHead->MoveToLeft();

        if(action == 3) SnakeHead->MoveToRight();

    }
    FVector TailLocation;
    FVector HeadLocation;

    float MoveStepSize;
    int direction;
    TArray<AActor *> Head2;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeHead::StaticClass(), Head2);
    for (AActor *h : Head2)
    {

        ASnakeHead *SnakeHead = Cast<ASnakeHead>(h);
        HeadLocation = SnakeHead->GetActorLocation();
        MoveStepSize = SnakeHead->GetMoveStepSize();

        // Get the snake direction
        //! left = 1, right = 2, up = 3, down = 4
        if(SnakeHead->DirLeft)
            direction = 1;
        if(SnakeHead->DirRight)
            direction = 2;
        if(SnakeHead->DirUp)
            direction = 3;
        if(SnakeHead->DirDown)
            direction = 4;
    }

    // snake head position fix
    if(direction == 1)
        HeadLocation.Y -= 100;
    if(direction == 2)
        HeadLocation.Y += 100;
    if(direction == 3)
        HeadLocation.X += 100;
    if(direction == 4)
        HeadLocation.X -= 100;

    //! left = 1, right = 2, up = 3, down = 4
    //TODO repair if(true) statement (states broken)
    TArray<AActor *> Tails;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATail::StaticClass(), Tails);
    for (AActor* t : Tails)
    {
        TailLocation = t->GetActorLocation();
        UE_LOG(LogTemp, Warning, TEXT("Head Location: %f, %f"), HeadLocation.X, HeadLocation.Y);
        UE_LOG(LogTemp, Warning, TEXT("Tail location: %f, %f"), TailLocation.X, TailLocation.Y);
        if(direction == 1)
        {
            if(HeadLocation.X == TailLocation.X && HeadLocation.Y - MoveStepSize == TailLocation.Y)
                TailFront = true;

            if(HeadLocation.X - MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailLeft = true;

            if(HeadLocation.X + MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailRight = true;

            UE_LOG(LogTemp, Warning, TEXT("direction: %i"), direction);
        }
        if(direction == 2)
        {
            if(HeadLocation.X == TailLocation.X && HeadLocation.Y + MoveStepSize == TailLocation.Y)
                TailFront = true;
            if(HeadLocation.X - MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailRight = true;
            if(HeadLocation.X + MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailLeft = true;

            UE_LOG(LogTemp, Warning, TEXT("direction: %i"), direction);
        }
        if(direction == 3)
        {
            if(HeadLocation.X + MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailFront = true;
            if(HeadLocation.X == TailLocation.X && HeadLocation.Y - MoveStepSize == TailLocation.Y)
                TailLeft = true;
            if(HeadLocation.X == TailLocation.X && HeadLocation.Y + MoveStepSize == TailLocation.Y)
                TailRight = true;

            UE_LOG(LogTemp, Warning, TEXT("direction: %i"), direction);
        }
        if(direction == 4)
        {
            if(HeadLocation.X - MoveStepSize == TailLocation.X && HeadLocation.Y == TailLocation.Y)
                TailFront = true;
            if(HeadLocation.X == TailLocation.X && TailLocation.Y - MoveStepSize == TailLocation.Y)
                TailRight = true;
            if(HeadLocation.X == TailLocation.X && TailLocation.Y + MoveStepSize == TailLocation.Y)
                TailLeft = true;

            UE_LOG(LogTemp, Warning, TEXT("direction: %i"), direction);
        }
    }
UE_LOG(LogTemp, Warning, TEXT("Head Location: %f, %f"), HeadLocation.X, HeadLocation.Y);
UE_LOG(LogTemp, Warning, TEXT("direction: %i"), direction);
    // Observe the next state
    if(HeadLocation.X <= -900 && HeadLocation.Y <= -900)
    {
        if(direction == 4)
            nextState = 5;
        if(direction == 1)
            nextState = 4;
    }
    else if(HeadLocation.X <= -900 && HeadLocation.Y >= 900)
    {
        if(direction == 4)
            nextState = 4;
        if(direction == 2)
            nextState = 5;
    }
    else if(HeadLocation.X >= 900 && HeadLocation.Y <= -900)
    {
        if(direction == 1)
            nextState = 5;
        if(direction == 3)
            nextState = 4;
    }
    else if(HeadLocation.X >= 900 && HeadLocation.Y >= 900)
    {
        if(direction == 2)
            nextState = 4;
        if(direction == 3)
            nextState = 5;
    }
    else if(HeadLocation.X <= -900)
    {
        if(direction == 1)
            nextState = 3;
        else if(direction == 2)
            nextState = 2;
        else if(direction == 4)
            nextState = 1;
    }
    else if(HeadLocation.X >= 900)
    {
        if(direction == 1)
            nextState = 2;
        else if(direction == 2)
            nextState = 3;
        else if(direction == 3)
            nextState = 1;
    }
    else if(HeadLocation.Y >= 900)
    {
        if(direction == 2)
            nextState = 1;
        else if(direction == 3)
            nextState = 2;
        else if(direction == 4)
            nextState = 3;
    }
    else if(HeadLocation.Y <= -900)
    {
        if(direction == 1)
            nextState = 1;
        else if(direction == 3)
            nextState = 3;
        else if(direction == 4)
            nextState = 2;
    }
    else if(TailFront && TailLeft)
    {
        nextState = 4;
    }
    else if(TailFront && TailRight)
    {
        nextState = 5;
    }
    else if(TailFront)
    {
        nextState = 1;
    }
    else if(TailRight)
    {
        nextState = 2;
    }
    else if(TailLeft)
    {
        nextState = 3;
    }

    // food states
    else if(FruitLocation.X - HeadLocation.X == 0 && FruitLocation.Y - HeadLocation.Y > 0)
    {
        nextState = 8; //food up
    }
    else if(FruitLocation.X - HeadLocation.X == 0 && FruitLocation.Y - HeadLocation.Y < 0)
    {
        nextState = 9; // food down
    }
    else if(FruitLocation.X - HeadLocation.X > 0 && FruitLocation.Y - HeadLocation.Y == 0)
    {
        nextState = 7; // food right
    }
    else if(FruitLocation.X - HeadLocation.X < 0 && FruitLocation.Y - HeadLocation.Y == 0)
    {
        nextState = 6; // food left
    }

    else if(FruitLocation.X - HeadLocation.X > 0 && FruitLocation.Y - HeadLocation.Y < 0)
    {
        nextState = 13; // food right down
    }
    else if(FruitLocation.X - HeadLocation.X < 0 && FruitLocation.Y - HeadLocation.Y < 0)
    {
        nextState = 12; // food left down
    }
    else if(FruitLocation.X - HeadLocation.X > 0 && FruitLocation.Y - HeadLocation.Y > 0)
    {
        nextState = 11; // food right up
    }
    else if(FruitLocation.X - HeadLocation.X < 0 && FruitLocation.Y - HeadLocation.Y > 0)
    {
        nextState = 10; // food left up
    }

    TailFront = false;
	TailLeft = false;
	TailRight = false;

    // Observe the reward
    if(HitWallOrItself)
    {
        reward = gameOverReward;
        HitWallOrItself = false;
    }
    else if(AteFruit)
        {
            UE_LOG(LogTemp, Warning, TEXT("Ate fruit!"));
            reward = foodReward;
            AteFruit = false;
            steps = 0;
        }
        else
        {
            reward = moveReward;
        }

}

void ASnakeGameQLearningGameModeBase::SetReward(bool _AteFruit)
{
        AteFruit = _AteFruit;
}

void ASnakeGameQLearningGameModeBase::SaveQTable(std::map<std::pair<int, int>, float> QTable)
{
    std::ofstream f;
    f.open("qtable.txt", std::ios::out | std::ios::trunc);
    for (int i = 1; i <= numStates; i++)
    {
        for (int j = 1; j <= numActions; j++)
        {
            std::pair<int, int> stateActionPair = std::make_pair(i, j);
            UE_LOG(LogTemp, Warning, TEXT("Q[(%i, %i)] = %f"), i, j, Q[stateActionPair]);
            f << Q[stateActionPair] << '\n';
        }
    }
    f.close();
    // UE_LOG(LogTemp, Warning, TEXT("Q-table saved to file"));
}
