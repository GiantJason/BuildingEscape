// Cpoyright GiantJason 2024


#include "OpenDoor.h"

#define OPENLIMIT 50.f
#define OUT

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

	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("%s missing pressure plate."), *GetOwner()->GetName());
	}
}

//open door function
void UOpenDoor::OpenDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

//open door function
void UOpenDoor::CloseDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//poll the trigger volume every frame and count the mass on it to open the door
	if (GeTotalMassOfActorsOnPlate() > OPENLIMIT) {
		OpenDoor();

		DoorLastOpen = GetWorld()->GetRealTimeSeconds();//get the real time when the door is closed
	}

	//check if the door is ready to close
	if (DoorLastOpen + DoorCloseDelay <= GetWorld()->GetRealTimeSeconds()) {
		CloseDoor();
	}

}

float UOpenDoor::GeTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through the actos and sum their mass
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%f kg on the pressure plate."), TotalMass);
	}
	return TotalMass;
}

