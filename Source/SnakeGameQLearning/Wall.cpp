// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

AWall::AWall()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if(MeshForMesh.Succeeded())
	{
		Mesh->SetStaticMesh(MeshForMesh.Object);
        Mesh->SetRelativeScale3D(FVector(1)/ MeshForMesh.Object->GetBoundingBox().GetSize());
        Mesh->CastShadow = 0;
    }

    ConstructorHelpers::FObjectFinder<UMaterial> MaterialForMesh(TEXT("/Game/StarterContent/Materials/M_CobbleStone_Smooth"));
	if(MaterialForMesh.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialForMesh.Object);
	}

}
