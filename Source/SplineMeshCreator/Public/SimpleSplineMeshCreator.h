// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "SimpleSplineMeshCreator.generated.h"

UCLASS()
class SPLINEMESHCREATOR_API ASimpleSplineMeshCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleSplineMeshCreator();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


	//Visual components
	UPROPERTY(BlueprintReadOnly, Category = "Mesh components", meta = (ToolTip = "The spline you want your mesh will follow"))
	USplineComponent* splineToFollow;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh components", meta = (ToolTip = "The main mesh that will be placed along the spine"))
	UStaticMesh* mainMesh = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh components", meta = (ToolTip = "The forward Axis of your main mesh"))
	TEnumAsByte<ESplineMeshAxis::Type> mainAxis = ESplineMeshAxis::Z;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh components", meta = (ToolTip = "The max draw distance of the mesh"))
	float maxDrawDistance = 5000.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh components", meta = (ToolTip = "The mesh that will be placed at ends of spline meshes"))
	UStaticMesh* connectorMesh = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh components", meta = (ToolTip = "The transform for the connector for small edits", MakeEditWidget = true))
	FTransform connectorTransform;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Spline Mesh collision profile"))
	TEnumAsByte<ECollisionEnabled::Type> collisionProfile = ECollisionEnabled::QueryAndPhysics;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Whether or not to sperate meshes by Spline Points or by lenght"))
	bool seperateBySplinePoints = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Whether or not to place connectorMeshes at connection points"))
	bool placeConnectors = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Should there be a connector at the start of the spline"))
	bool startConnector = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Should there be a connector at the end of the spline"))
	bool endConnector = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Modifier to the meshes base lenght to force it to use more or less meshes when separating by length"))
	float lengthModifier = 20.f;
	float length = 1;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Generation Properties", meta = (ToolTip = "Whether you want the main mesh to be visible or not, to make it an invisible wall or not"))
	bool setInvisible = false;
	

	UFUNCTION()
	FVector getDistance(int pointIndex, bool byPoint);
	UFUNCTION()
	FVector getTangent(int pointIndex, bool byPoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void processSplineChanges();

};
