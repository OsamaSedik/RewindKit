// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RewindTimeComponent.generated.h"

UENUM(BlueprintType)
enum class ETimePhaseState : uint8
{
	ETP_Record		UMETA(DisplayName="Record"),
	ETP_Rewind		UMETA(DisplayName="Rewind"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimePhaseChanged,ETimePhaseState,TimePhaseState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REWINDKIT_API URewindTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URewindTimeComponent();
	
	// show debug arrow for the three axis ( X, Y , Z )
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Rewind Time")
	bool bShowDebugRecording = false;
	
	// the state of time phase -> used for apply effects & anim blueprint 
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Rewind Time")
	ETimePhaseState TimePhaseState = ETimePhaseState::ETP_Record;
	
	// delegate callback for anim blueprint to inverse the animation or not 
	UPROPERTY(BlueprintAssignable,Category="Rewind Time")
	FOnTimePhaseChanged OnTimePhaseChanged;
	
	// time for recording set between 1 and 3 for performance 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Rewind Time", meta=(ClampMin="1.0", ClampMax="5.0", UIMin="1.0", UIMax="5.0"))
	float RecordTime = 2.0f;
	
	// an array to store the actor Positions */
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Rewind Time")
	TArray<FVector> RecordedPositions;

	// an array to store the actor Rotations */
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Rewind Time")
	TArray<FRotator> RecordedRotations;

	// timers for record & rewind 
	FTimerHandle RecordTimerHandle;
	FTimerHandle RewindTimerHandle;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// draw debug axis for the actor 
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void DrawDebugRecord();
	
public:
	// start recording and call RecordMovement() by a timer 
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void StartRecordMovement();
	
	/** a Function to record data into the positions and rotations */
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void RecordMovement();
	
	// stop the recording timer handle 
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void StopRecordMovement();
	
	// start rewinding and call RewindMovement() by a timer 
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void StartRewindMovement();
	
	/** a Function to reimport the data from the positions and rotations into the owner */
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void RewindMovement();
	
	// stop the rewind timer 
	UFUNCTION(BlueprintCallable,Category=" Rewind Time")
	void StopRewindMovement();
};
