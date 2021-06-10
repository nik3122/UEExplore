#include "Components/XHoverComponent.h"

UXHoverComponent::UXHoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UXHoverComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UXHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Owner = GetOwner();
	const float RunningTime = Owner->GetGameTimeSinceCreation();
	const float DeltaHeight = (FMath::Sin(DeltaTime + RunningTime) - FMath::Sin(RunningTime));
	FVector NewLocation = Owner->GetActorLocation();
	NewLocation.Z += DeltaHeight * 20.0f;
	Owner->SetActorLocation(NewLocation);
}

