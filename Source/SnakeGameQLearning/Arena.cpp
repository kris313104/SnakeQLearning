// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena.h"
#include "Wall.h"

AArena::AArena()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    if(MeshForMesh.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(MeshForMesh.Object);
        GetStaticMeshComponent()->SetRelativeScale3D(FVector(1)/ MeshForMesh.Object->GetBoundingBox().GetSize());
	}
    GetStaticMeshComponent()->CastShadow = 0;

    WallHeight = 200;
    WallWidth = 50;
}

void AArena::BeginPlay()
{
    Super::BeginPlay();
    FVector Origin;
    FVector Extend;
    GetActorBounds(false, Origin, Extend);

    FVector LeftWallLoc = Origin - FVector(Extend.X, 0, 0);
    FVector VerticalWallScale = FVector(WallWidth, Extend.Y * 2, WallHeight);
    SpawnWall(LeftWallLoc, VerticalWallScale);

    FVector RightWallLoc = Origin + FVector(Extend.X, 0, 0);
    SpawnWall(RightWallLoc, VerticalWallScale);

    FVector UpWallLoc = Origin + FVector(0, Extend.Y, 0);
    FVector HorizontalWallScale = FVector(Extend.X * 2, WallWidth, WallHeight);
    SpawnWall(UpWallLoc, HorizontalWallScale);

    FVector DownWallLoc = Origin - FVector(0, Extend.Y, 0);
    SpawnWall(DownWallLoc, HorizontalWallScale);
}

void AArena::SpawnWall(FVector SpawnLocation, FVector SpawnScale)
{
    FTransform SpawnTransform = FTransform(FRotator(), SpawnLocation, SpawnScale);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ADeadZone* SpawnedWall = GetWorld()->SpawnActor<AWall>(AWall::StaticClass(), SpawnTransform, FActorSpawnParameters());

}

void AArena::GetRandomFruitSpawnLoc(FVector& OutLocation)
{
    FVector Origin;
    FVector Extend;
    GetActorBounds(false, Origin, Extend);
    int SpawnZ = 10;
    int SpawnX = FMath::RandRange(Origin.X - Extend.X + WallWidth, Origin.X + Extend.X - WallWidth) / 200;
    int SpawnY = FMath::RandRange(Origin.Y - Extend.Y + WallWidth, Origin.Y + Extend.Y - WallWidth) / 200;

    UE_LOG(LogTemp, Warning, TEXT("SpawnZ = %d"), SpawnZ);
    UE_LOG(LogTemp, Warning, TEXT("SpawnX = %d"), SpawnX);
    UE_LOG(LogTemp, Warning, TEXT("SpawnY = %d"), SpawnY);

    OutLocation = FVector(SpawnX * 200, SpawnY * 200, SpawnZ);
}
