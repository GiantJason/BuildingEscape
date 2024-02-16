// Cpoyright GiantJason 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

#include "Grabber.generated.h"

#define RAY_DISTANCE 100.f;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// the distance of player view can really reach
	float RayDistance = RAY_DISTANCE; 

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// find (assumed) attached physics handle component
	void FindPhysicsHandleComponent(); 

	// InputComponent only appears at run time
	UInputComponent* InputComponent = nullptr;

	// setup (assumed) attached input component
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// grab an object
	void Grab();

	//called when grab key is released
	void Release();
};
