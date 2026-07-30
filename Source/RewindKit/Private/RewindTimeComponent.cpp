// Fill out your copyright notice in the Description page of Project Settings.


#include "RewindTimeComponent.h"
#include "Engine/World.h"        
#include "TimerManager.h"        
#include "GameFramework/Actor.h" 
#include "DrawDebugHelpers.h"    

// Sets default values for this component's properties
URewindTimeComponent::URewindTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void URewindTimeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URewindTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URewindTimeComponent::StartRecordMovement()
{
	StopRewindMovement();
	TimePhaseState = ETimePhaseState::ETP_Record;
	OnTimePhaseChanged.Broadcast(TimePhaseState);
	GetWorld()->GetTimerManager().SetTimer(RecordTimerHandle,this,&URewindTimeComponent::RecordMovement,0.016f,true);
}

void URewindTimeComponent::StopRecordMovement()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(RecordTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RecordTimerHandle);
	}
}

void URewindTimeComponent::RecordMovement()
{
	/** first check if the numbers of elements for the two arrays are grater than record time divided on the tick delta seconds */
	const int32 Max_Positions_InTime = FMath::RoundToInt(RecordTime/ GetWorld()->GetDeltaSeconds());
	const int32 Max_Rotations_InTime = FMath::RoundToInt(RecordTime/ GetWorld()->GetDeltaSeconds());
	
	if (RecordedPositions.Num()> Max_Positions_InTime && RecordedRotations.Num()>Max_Rotations_InTime )
	{
		/* remove the elements at last index to free up the data */
		RecordedPositions.RemoveAt(RecordedPositions.Num() -1);
		RecordedRotations.RemoveAt(RecordedRotations.Num() -1);
	}
	else
	{
		/* insert the owner location and rotation into the arrays */
		RecordedPositions.Insert(GetOwner()->GetActorLocation(),0);
		RecordedRotations.Insert(GetOwner()->GetActorRotation(),0);
		DrawDebugRecord();
	}
}


void URewindTimeComponent::StartRewindMovement()
{
	StopRecordMovement();
	TimePhaseState = ETimePhaseState::ETP_Rewind;
	OnTimePhaseChanged.Broadcast(TimePhaseState);
	GetWorld()->GetTimerManager().SetTimer(RewindTimerHandle,this,&URewindTimeComponent::RewindMovement,0.016f,true);
}

void URewindTimeComponent::StopRewindMovement()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(RewindTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RewindTimerHandle);
	}
}

void URewindTimeComponent::RewindMovement()
{
	// if there are more data in the TArray for both Positions and Rotations
	if (RecordedPositions.Num() > 0 && RecordedRotations.Num() > 0)
	{
		this->GetOwner()->SetActorLocation(RecordedPositions[0]);
		this->GetOwner()->SetActorRotation(RecordedRotations[0]);

		DrawDebugRecord();
		/** remove the data that have been rewinded from the arrays */
		RecordedPositions.RemoveAt(0);
		RecordedRotations.RemoveAt(0);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Finished Rewind"));
		GetWorld()->GetTimerManager().ClearTimer(RewindTimerHandle);
	}
}


void URewindTimeComponent::DrawDebugRecord()
{
	if (!bShowDebugRecording || RecordedPositions.Num() < 2) return;

	UWorld* World = GetWorld();
	if (!World) return;

	constexpr float LifeTime = 0.0f;
	constexpr float AxisLength = 50.0f;
	constexpr float ArrowSize = 15.0f;
	constexpr float Thickness = 1.5f;

	for (int32 i = 0; i < RecordedPositions.Num(); i += 5) // skip for performance
	{
		const FVector& Pos = RecordedPositions[i];
		const FRotator& Rot = RecordedRotations[i];

		// X-axis (Forward)
		const FVector Forward = Rot.Vector();
		DrawDebugDirectionalArrow(World,Pos,Pos + Forward * AxisLength,ArrowSize,
			FColor::Red,false,LifeTime,0,Thickness);
		
		
		// Y axis (Right)
		const FVector Right = Rot.RotateVector(FVector::RightVector);
		DrawDebugDirectionalArrow(World,Pos,Pos + Right * AxisLength,	ArrowSize
			,FColor::Green,false,LifeTime,0,Thickness);
		

		// Z axis (Up)
		const FVector Up = Rot.RotateVector(FVector::UpVector);
		DrawDebugDirectionalArrow(World,Pos,Pos + Up * AxisLength,ArrowSize,
			FColor::Blue,false,LifeTime,0,Thickness);
	}
}




