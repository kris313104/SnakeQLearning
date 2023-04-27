// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"
#include "Tail.h"
#include "SnakeGameQLearningGameModeBase.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1;
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("HeadMesh");
	SetRootComponent(RootComp);
	HeadMesh->SetupAttachment(RootComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if(MeshForHeadMesh.Succeeded())
	{
		HeadMesh->SetStaticMesh(MeshForHeadMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialForMesh(TEXT("/Game/StarterContent/Materials/M_Wood_Walnut"));
	if(MaterialForMesh.Succeeded())
	{
		HeadMesh->SetMaterial(0, MaterialForMesh.Object);
	}
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HeadMesh->SetCollisionObjectType(ECC_Pawn);
	HeadMesh->SetCollisionResponseToChannels(ECR_Overlap);
	HeadMesh->CastShadow = 0;

	Score = 0;


}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();
	MoveStepSize = HeadMesh->GetStaticMesh()->GetBoundingBox().GetSize().X;
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector PreviousSectionLoc = HeadMesh->GetComponentLocation();
	AddActorWorldOffset(MoveDir * MoveStepSize);
	for (int i = 0; i < Tails.Num(); i++)
	{
		FVector CurrentSectionLoc = Tails[i]->GetActorLocation();
		Tails[i]->SetActorLocation(PreviousSectionLoc);
		PreviousSectionLoc = CurrentSectionLoc;

	}
}

// Called to bind functionality to input
// void ASnakeHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// 	ensure(PlayerInputComponent);

// 	PlayerInputComponent->BindAction("SaveQTable", IE_Pressed, this, &ASnakeGameQLearningGameModeBase::SaveQTable);
// }

void ASnakeHead::MoveForward()
{
	if(DirUp)
	{
		MoveUp();
	}

	else if(DirLeft)
	{
		MoveLeft();
	}

	else if(DirRight)
	{
		MoveRight();
	}

	else if(DirDown)
	{
		MoveDown();
	}
	else
	{
	MoveDir = FVector(1, 0, 0);
	DirRight = false;
	DirLeft = false;
	DirUp = true;
	DirDown = false;
	}
}

void ASnakeHead::MoveToLeft()
{
	if(DirUp)
	{
		MoveLeft();
	}

	else if(DirLeft)
	{
		MoveDown();
	}

	else if(DirRight)
	{
		MoveUp();
	}

	else if(DirDown)
	{
		MoveRight();
	}
	else
	{
	MoveDir = FVector(0, -1, 0);
	DirRight = false;
	DirLeft = true;
	DirUp = false;
	DirDown = false;
	}
}

void ASnakeHead::MoveToRight()
{
	if(DirUp)
	{
		MoveRight();
	}

	else if(DirLeft)
	{
		MoveUp();
	}

	else if(DirRight)
	{
		MoveDown();
	}

	else if(DirDown)
	{
		MoveLeft();
	}
	else
	{
	MoveDir = FVector(0, 1, 0);
	DirRight = true;
	DirLeft = false;
	DirUp = false;
	DirDown = false;
	}
}

void ASnakeHead::MoveUp()
{
	DirDown = false;
	DirLeft = false;
	DirRight = false;

	DirUp = true;
	MoveDir = FVector(1, 0, 0);
}

void ASnakeHead::MoveDown()
{
	DirUp = false;
	DirLeft = false;
	DirRight = false;

	DirDown = true;
	MoveDir = FVector(-1, 0, 0);
}

void ASnakeHead::MoveLeft()
{
	DirUp = false;
	DirDown = false;
	DirRight = false;

	DirLeft = true;
	MoveDir = FVector(0, -1, 0);
}

void ASnakeHead::MoveRight()
{
	DirUp = false;
	DirLeft = false;
	DirDown = false;

	DirRight = true;
	MoveDir = FVector(0, 1, 0);
}

void ASnakeHead::EatFruit()
{
	Score++;
	UE_LOG(LogTemp, Warning, TEXT("Score is now: %i"), Score);
	FVector TailSpawnLoc = HeadMesh->GetComponentLocation() - MoveDir * MoveStepSize;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATail *NewTail = GetWorld()->SpawnActor<ATail>(ATail::StaticClass(), TailSpawnLoc, FRotator(), SpawnParams);

	if(NewTail)
	{
		Tails.Add(NewTail);
	}
}

void ASnakeHead::RemoveTails()
{
	Tails.Empty();
}

float ASnakeHead::GetMoveStepSize()
{
	return MoveStepSize;
}
