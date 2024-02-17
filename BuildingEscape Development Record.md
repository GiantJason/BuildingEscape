#### BuildingEscape Development Record

##### 1. Actor that opens the door(get the pawn as an actor to open the door)

	1) get the pawn as actors 

```C++
	//get the pawn as ActorOpens
	//TODO count total mass that is overlapping trigger volume 
	ActorOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("ActorOpens get."));
```

2. trigger volume that opens the door

```c++
	//pressure plate define(designated manually in UE editor)
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

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
```

