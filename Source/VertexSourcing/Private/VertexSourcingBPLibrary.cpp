#include "VertexSourcingBPLibrary.h"
#include "VertexSourcing.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/ColorVertexBuffer.h"
#include "RHICommandList.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "HAL/PlatformProcess.h"

#include "SkeletalRenderPublic.h"

UVertexSourcingBPLibrary::UVertexSourcingBPLibrary(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{

}

void UVertexSourcingBPLibrary::ChangeVertexColor(UMeshComponent* MeshComponent, FColor NewColor, const TArray<FVector>& Vertices, float DistanceThreshold)
{
  if (!MeshComponent || !MeshComponent->IsValidLowLevelFast())
    return;

  if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent))
  {


    // Component LOD
    StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
    FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];
    InstanceMeshLODInfo->PaintedVertices.Empty();
    InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;



    // Asset LOD Model
    FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];

    InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::Black, LODModel.GetNumVertices());



    for (const FVector& ParticlePos : Vertices)
    {

      // Modify vertex colors based on the map
      for (uint32 Index = 0; Index < LODModel.VertexBuffers.PositionVertexBuffer.GetNumVertices(); Index++)
      {
        FVector NearPos = FVector(LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(Index));
        float Distance = FVector::Distance(ParticlePos, NearPos);
        if (Distance < DistanceThreshold)
        {


          // Set the new color for the vertex
          InstanceMeshLODInfo->OverrideVertexColors->VertexColor(Index) = NewColor;
        }
      }
    }


    // Sleep to introduce a small delay
    FPlatformProcess::Sleep(0.005f);

    // Initialize resource
    BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);

    // Mark render state dirty
    StaticMeshComponent->MarkRenderStateDirty();
  }

  else if (USkinnedMeshComponent* SkinnedMeshComponent = Cast<USkinnedMeshComponent>(MeshComponent))
  {
    for (int32 LODIndex = 0; LODIndex < SkinnedMeshComponent->GetNumLODs(); ++LODIndex)
    {
      FSkelMeshComponentLODInfo* InstanceSkinnedMeshLODInfo = &SkinnedMeshComponent->LODInfo[LODIndex];
      InstanceSkinnedMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;

      FSkeletalMeshLODRenderData& LODResource = SkinnedMeshComponent->GetSkinnedAsset()->GetResourceForRendering()->LODRenderData[LODIndex];

      InstanceSkinnedMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::Black, LODResource.GetNumVertices());



      for (const FVector& Pos : Vertices)
      {

        // Modify vertex colors based on the map
        for (uint32 Index = 0; Index < LODResource.GetNumVertices(); Index++)
        {
          FVector NearPos = FVector(LODResource.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(Index));
          float Distance = FVector::Distance(Pos, NearPos);
          if (Distance < DistanceThreshold)
          {

            InstanceSkinnedMeshLODInfo->OverrideVertexColors->VertexColor(Index) = NewColor;

          }
        }
      }
      FPlatformProcess::Sleep(0.005f);
      BeginInitResource(InstanceSkinnedMeshLODInfo->OverrideVertexColors);
    }

    SkinnedMeshComponent->MarkRenderStateDirty();
  }
}


int32 UVertexSourcingBPLibrary::ReturnVertexCount(UStaticMeshComponent* StaticMeshComponent)
{

  if (!StaticMeshComponent || !StaticMeshComponent->IsValidLowLevelFast())
    return -1;


  // Component LOD
  StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());

  FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
  int32 VertecisNum = LODModel.GetNumVertices();
  return VertecisNum;


}