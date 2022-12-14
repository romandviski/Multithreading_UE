// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "HAL/ThreadingBase.h"

#include "SynPrim/SimpleAtomic_Runnable.h"
#include "SynPrim/SimpleCounter_Runnable.h"

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

	// SimpleCounter Settings
	UPROPERTY(BlueprintReadWrite, Category = "SimpleCounter Setting")
	bool bIsUseSafeVariable = true;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleCounter Setting")
	FColor ColorSimpleCounter;
	class FSimpleCounter_Runnable *MyRunnableClass_SimpleCounter = nullptr;
	FRunnableThread *CurrentRunningGameModeThread_SimpleCounter = nullptr;
	// SimpleCounter control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleCounterThread();
	UFUNCTION(BlueprintCallable) // доступно для виндовс платформ 
	bool SwitchRunStateSimpleCounterThread(bool bIsPause);
	UFUNCTION(BlueprintCallable)
	void StopSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	void KillSimpleCounterThread(bool bIsShouldWait = true);
	UFUNCTION(BlueprintCallable)
	int64 GetSimpleCounterThread();

	
	//SimpleAtomic Settings
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
