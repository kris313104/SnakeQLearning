// Fill out your copyright notice in the Description page of Project Settings.


#include "Tail.h"
#include "SnakeHead.h"

ATail::ATail()
{
    Mesh->SetRelativeScale3D(FVector(0.6));
}

void ATail::BeginPlay()
{
    Super::BeginPlay();
    ASnakeHead *SnakeHeadOwner = Cast<ASnakeHead>(GetOwner());
    if(SnakeHeadOwner)
    {
        Mesh->SetStaticMesh(SnakeHeadOwner->HeadMesh->GetStaticMesh());
    }
}


