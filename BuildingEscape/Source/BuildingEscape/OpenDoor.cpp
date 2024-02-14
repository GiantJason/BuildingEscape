// Cpoyright GiantJason 2024


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//get the pawn as ActorOpens
	ActorOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("ActorOpens get."));

	// find the owner-door
	DoorOwner = GetOwner();
}

//open door function
void UOpenDoor::OpenDoor()
{

	//create the rotator
	FRotator DoorROotation = FRotator(0.0f, OpenAngle, 0.0f);

	//set the door rotation
	DoorOwner->SetActorRotation(DoorROotation);
}

//open door function
void UOpenDoor::CloseDoor()
{

	//create the rotator
	FRotator DoorROotation = FRotator(0.0f, 0.0f, 0.0f);

	//set the door rotation
	DoorOwner->SetActorRotation(DoorROotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//poll the trigger volume every frame
	//if the actor that opens is in the volume, then open the door
	if (PressurePlate->IsOverlappingActor(ActorOpens)) {
		OpenDoor();

		DoorLastOpen = GetWorld()->GetRealTimeSeconds();//get the real time when the door is closed
	}

	//check if the door is ready to close
	if (DoorLastOpen + DoorCloseDelay <= GetWorld()->GetRealTimeSeconds()) {
		CloseDoor();
	}

}

