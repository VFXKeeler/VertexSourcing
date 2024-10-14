// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "ProceduralMeshComponent.h"
#include "Rendering/ColorVertexBuffer.h"
#include "StaticMeshComponentLODInfo.h"
#include "RenderResource.h"
#include "Rendering/PositionVertexBuffer.h"
#include "RHIResources.h"
#include "RHIContext.h"
#include "DynamicRHI.h"
#include "VertexSourcingBPLibrary.generated.h"



UCLASS()
class UVertexSourcingBPLibrary : public UBlueprintFunctionLibrary
{

  GENERATED_UCLASS_BODY()

  UFUNCTION(BlueprintCallable, Category = "VertexReColor")
  static void ChangeVertexColor(UMeshComponent* MeshComponent, FColor NewColor, const TArray<FVector>& Vertices, float DistanceThreshold);

  UFUNCTION(BlueprintCallable, Category = "VertexSource")
  static int32 ReturnVertexCount(UStaticMeshComponent* StaticMeshComponent);


};
