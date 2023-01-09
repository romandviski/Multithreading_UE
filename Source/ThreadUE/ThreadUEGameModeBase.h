// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "HAL/ThreadingBase.h"

#include "ThreadUEGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class THREADUE_API AThreadUEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	//SimpleAtomic Setting
	TArray<FRunnableThread*> CurrentRunnableGameModeThread_SimpleAtomic;
	
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	int32 IterationRunnableCircle = 100000;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	int32 NumberOfThreadToCreate = 12;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic Setting")
	bool bUseAtomic = true;
	
	//SimpleAtomic control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleAtomicThread();
	UFUNCTION(BlueprintCallable)
	void GetCounterSimpleAtomic(int32 &Atomic1, int32 &Atomic2, int32 &NonAtomic1, int32 &NonAtomic2);
	UFUNCTION(BlueprintCallable)
	void ResetCounterSimpleAtomic();
	
	//SimpleAtomic Storage
	std::atomic_int16_t AtomicCounter1;
	std::atomic_int16_t AtomicCounter2;
	int16 NonAtomicCounter1;
	int16 NonAtomicCounter2;
};
