// Copyright © Emily Peregrine 2016

#include "Escape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	FindInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
}

void UGrabber::FindPhysicsHandleComponent(){
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing a Physics Handle Component"), *(GetOwner()->GetName()));
	}

    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
}

void UGrabber::FindInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing an Input Component"), *(GetOwner()->GetName()));
	}
	else {
		//Bind action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}


void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbed Object"));

	//Try and reach any actors with physics body collision
	GetFirstPhysicsBodyInReach();
	// If we hit something attach a physics handle
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Released Object"));
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//if the physics handle is attached
		//move he object we're holding

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	/// Ray-cast out to reach distance
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Range;
	FVector LookVector = PlayerViewPointLocation + LineTraceDirection;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LookVector,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
		);

	/// Setup Query parameters
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LookVector,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	///See what we hit
	AActor* HitActor = Hit.GetActor();

	if (HitActor) {
		FString HitActorName = HitActor->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitActorName);
	}

	return FHitResult();
}