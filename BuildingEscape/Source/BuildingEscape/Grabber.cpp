// Cpoyright GiantJason 2024


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	UE_LOG(LogTemp, Warning, TEXT("haha"));
	/// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object we are holding 
		PhysicsHandle->SetTargetLocation(GetReachLine(LineBothEnds::END));
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		// log our an error of not finding physics handle
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle!"), *GetOwner()->GetName());
	}
}

// setup (assumed) attached input component 
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		if (!InputComponent) { return; }
		// bind the input
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		// log our an error of not finding Input component
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component!"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	
	FHitResult Hit;
	// set up parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLine(LineBothEnds::START),
		GetReachLine(LineBothEnds::END),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// find out the object we`ve hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		FString ActorName = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("The actor hit is: %s"), *ActorName);
	}
	return Hit;
}

void UGrabber::Grab()
{
	/// line trace and see if we reach any actors with physics collision body set 
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // get the mesh in our case
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		/// if we hit sth, attach it with physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator::ZeroRotator);
	}
}

void UGrabber::Release()
{
	// release the object we are holding
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLine(LineBothEnds Line)
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		//macron OUT does nothing but remind us that changing variables here is not appropriate
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// log out to test (tips: remember to de-reference the string)
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotation.ToString()
	//);

	//draw a debug line to visualize
 	// FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * RayDistance;
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation, LineTraceEnd,
	//	FColor(255, 0, 0), // pure red line
	//	false, 0.f, 0.f, 10.f
	//);
	// ray-cast out to reach distance

	switch (Line) {
		case LineBothEnds::START:
			return PlayerViewPointLocation;
		case LineBothEnds::END:
			return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * RayDistance;
	}
	// prevent potential bug
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * RayDistance;
}

