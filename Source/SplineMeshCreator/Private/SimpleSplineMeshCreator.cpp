// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleSplineMeshCreator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASimpleSplineMeshCreator::ASimpleSplineMeshCreator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	splineToFollow = CreateDefaultSubobject<USplineComponent>("MySpline");
}

void ASimpleSplineMeshCreator::processSplineChanges()
{
	if(!IsValid(connectorMesh) || !IsValid(mainMesh))
		return;

	//Finding the last inddex for our for loop
	int finalIndex = splineToFollow->GetNumberOfSplinePoints() - 1;

	if (!seperateBySplinePoints)
	{
		float splineLenght = splineToFollow->GetSplineLength();

		FVector meshExtents = mainMesh->GetBounds().BoxExtent;
		float max = UKismetMathLibrary::Max(meshExtents.X, meshExtents.Z);
		max = UKismetMathLibrary::Max(max, meshExtents.Y);
		length = max * lengthModifier;
		finalIndex = UKismetMathLibrary::FTrunc(splineLenght / (length));
	}

	//Start spawning meshes along splineToFollow
	for (int i = 0; i <= finalIndex; ++i)
	{
		USplineMeshComponent* newSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		newSplineMesh->SetStaticMesh(mainMesh);
		newSplineMesh->SetForwardAxis(mainAxis);
		newSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		newSplineMesh->Mobility = EComponentMobility::Static;
		newSplineMesh->LDMaxDrawDistance = maxDrawDistance;
		newSplineMesh->SetVisibility(!setInvisible);
		FinishAndRegisterComponent(newSplineMesh);



		FVector startPoint = getDistance(i, seperateBySplinePoints);
		FVector startTangent = getTangent(i, seperateBySplinePoints);

		FVector endPoint = getDistance(i+1, seperateBySplinePoints);
		FVector endTangent = getTangent(i+1, seperateBySplinePoints);

		newSplineMesh->SetStartAndEnd(startPoint, startTangent, endPoint, endTangent);

		newSplineMesh->SetCollisionEnabled(collisionProfile);
		newSplineMesh->SetCollisionResponseToChannels(collisionsResponses);

		//Place connector after new spline mesh is made
		if (placeConnectors && !setInvisible)
		{
			if (!startConnector && i == 0)
			{
				continue;
			}
			else if (!endConnector && i == finalIndex)
			{
				continue;
			}

			UStaticMeshComponent* newConnector = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
			newConnector->SetStaticMesh(connectorMesh);
			newConnector->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			newConnector->Mobility = EComponentMobility::Static;
			newConnector->SetCachedMaxDrawDistance(maxDrawDistance);
			newConnector->SetCollisionEnabled(collisionProfile);
			FinishAndRegisterComponent(newConnector);

			FVector newLoc;
			FRotator newRot;
			FTransform newTransform;

			if (i == 0)
			{
				newRot = UKismetMathLibrary::FindLookAtRotation(startPoint, endPoint);
			}
			else if (i == finalIndex)
			{
				newRot = UKismetMathLibrary::FindLookAtRotation(startPoint, getDistance(i-1, seperateBySplinePoints));
			}
			else if (i != 0 && i != finalIndex)
			{
				newLoc = getDistance(i-1, seperateBySplinePoints);
				newRot = UKismetMathLibrary::FindLookAtRotation(newLoc, endPoint);
			}

			newRot = UKismetMathLibrary::FindLookAtRotation(startPoint, endPoint);
			newTransform = UKismetMathLibrary::MakeTransform(startPoint, newRot + connectorTransform.Rotator(), connectorTransform.GetScale3D());
			newTransform += connectorTransform;
			newConnector->SetWorldTransform(newTransform);
		}
	}
}

// Called when the game starts or when spawned
void ASimpleSplineMeshCreator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleSplineMeshCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Just simply getting location based off distance or point to make construction easier
FVector ASimpleSplineMeshCreator::getDistance(int pointIndex, bool byPoint)
{
	FVector result = FVector(-1,-1,-1);

	if (byPoint)
	{
		return splineToFollow->GetWorldLocationAtSplinePoint(pointIndex);
	}
	else
	{
		float distance = length * pointIndex;
		return splineToFollow->GetWorldLocationAtDistanceAlongSpline(distance);
	}

	return result;
}

FVector ASimpleSplineMeshCreator::getTangent(int pointIndex, bool byPoint)
{
	FVector result;

	if (byPoint)
	{
		return splineToFollow->GetTangentAtSplinePoint(pointIndex, ESplineCoordinateSpace::World);
	}
	else
	{
		float distance = length * pointIndex;
		return splineToFollow->GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	}

	return result;
}
