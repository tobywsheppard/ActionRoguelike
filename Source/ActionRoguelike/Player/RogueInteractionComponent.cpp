#include "RogueInteractionComponent.h"

#include "Engine/OverlapResult.h"


URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



// Called every frame
void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	
	FVector Centre = PC->GetPawn()->GetActorLocation();
	
	
	
	ECollisionChannel CollisionChannel = ECC_Visibility;
	
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Centre, FQuat::Identity, CollisionChannel, Shape);
	
	DrawDebugSphere(GetWorld(), Centre, InteractionRadius, 32, FColor::White);
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.0;

	for (FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.0f), FColor::Red);
		
		FVector OverlapDirection = (OverlapLocation - Centre).GetSafeNormal();
		
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		
		FString DebugString = FString::Printf(TEXT("Dot: %f"),DotResult );
		
		DrawDebugString(GetWorld(),OverlapLocation, DebugString, nullptr, FColor::White, 0.0f, true);
		
		if (DotResult > HighestDotResult)
		{
			BestActor = Overlap.GetActor();
			HighestDotResult = DotResult;
		}
	}
	
	if (BestActor)
	{
		DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.0f), FColor::Green);
	}
	
}

