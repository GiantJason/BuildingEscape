// Cpoyright GiantJason 2024


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

#define OUT

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
	//UE_LOG(LogTemp, Warning, TEXT("Grabber is reporting for duty!"));

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get player view point every tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation //macron OUT does nothing but remind us that changing variables here is not appropriate
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * RayDistance;

	/// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object we are holding 
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// look for attached physics handle & InputComponent
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// physics handle is found

	}
	else {
		// log our an error of not finding physics handle
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle!"), *GetOwner()->GetName());
	}
}

// setup (assumed) attached input component 
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		// InputComponent is found
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"));

		/// bind the axis input
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

	// get player view point every tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation //macron OUT does nothing but remind us that changing variables here is not appropriate
	);

	// log out to test (tips: remember to de-reference the string)
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotation.ToString()
	//);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * RayDistance;

	//draw a debug line to visualize
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation, LineTraceEnd,
	//	FColor(255, 0, 0), // pure red line
	//	false, 0.f, 0.f, 10.f
	//);
	// ray-cast out to reach distance

	FHitResult Hit;
	// set up parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
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
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"));

	/// line trace and see if we reach any actors with physics collision body set 
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
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
	UE_LOG(LogTemp, Warning, TEXT("Grab key released!"));

	// release the object we are holding
	PhysicsHandle->ReleaseComponent();
}

