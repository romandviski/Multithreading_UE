// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThreadUEGameModeBase.h"
#include "SynPrim/SimpleAtomic_Runnable.h"

void AThreadUEGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//uint32 ThreadID = FPlatformTLS::GetCurrentThreadId();
	//UE_LOG(LogTemp, Error, TEXT("AThreadUEGameModeBase::Tick ID - %d, Name - %s"), ThreadID, *FThreadManager::GetThreadName(ThreadID));

	FThreadManager::Get().ForEachThread([&](uint32 ThreadID, FRunnableThread* Runnable) //FThreadManager - singleton, [&] - lambda function
	{
		//Runnable->GetThreadPriority();

		FString MyThreadType = "none";
		switch (Runnable->GetThreadPriority()) // switch by enum
		{
		case TPri_Normal:
			MyThreadType = "TPri_Normal"; // classic
			break;
		case TPri_AboveNormal:
			MyThreadType = "TPri_AboveNormal";
			break;
		case TPri_BelowNormal:
			MyThreadType = "TPri_BelowNormal";
			break;
		case TPri_Highest:
			MyThreadType = "TPri_Highest";
			break;
		case TPri_Lowest:
			MyThreadType = "TPri_Lowest";
			break;
		case TPri_SlightlyBelowNormal:
			MyThreadType = "TPri_SlightlyBelowNormal";
			break;
		case TPri_TimeCritical:
			MyThreadType = "TPri_TimeCritical";
			break;
		case TPri_Num:
			MyThreadType = "TPri_Num";
			break;
		default:
			break;
		}

		UE_LOG(LogTemp, Error, TEXT("AThreadUEGameModeBase::Tick ID - %d, Name - %s, Priority - %s"),
				ThreadID, *Runnable->GetThreadName(), *MyThreadType);
	});
}

void AThreadUEGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AThreadUEGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AThreadUEGameModeBase::CreateSimpleAtomicThread()
{
	for (int i = 0; i < NumberOfThreadToCreate; ++i)
	{
		bool bIsFlag = false;
		if (i % 2)
			bIsFlag = true;

		FColor Color;
		class FSimpleAtomic_Runnable *MyRunnableClass_SimpleAtomic = new FSimpleAtomic_Runnable(Color, this, IterationRunnableCircle, bIsFlag, bUseAtomic);
		CurrentRunnableGameModeThread_SimpleAtomic.Add(FRunnableThread::Create(MyRunnableClass_SimpleAtomic, TEXT("SimpleAtomic thread"), 0, EThreadPriority::TPri_Normal));
	}
}

void AThreadUEGameModeBase::GetCounterSimpleAtomic(int32& Atomic1, int32& Atomic2, int32& NonAtomic1, int32& NonAtomic2)
{
	if (bUseAtomic)
	{
		NonAtomic1 = FPlatformAtomics::AtomicRead(&NonAtomicCounter1);
		NonAtomic2 = FPlatformAtomics::AtomicRead(&NonAtomicCounter2);
		if (AtomicCounter1.is_lock_free())
		{
			Atomic1 = AtomicCounter1;
			//Atomic1 = AtomicCounter1.load();
		}
		if (AtomicCounter2.is_lock_free())
		{
			Atomic2 = AtomicCounter2;
		}
	}
	else
	{
		NonAtomic1 = NonAtomicCounter1;
		NonAtomic2 = NonAtomicCounter2;
	}
}

void AThreadUEGameModeBase::ResetCounterSimpleAtomic()
{
	NonAtomicCounter1 = 0;
	NonAtomicCounter2 = 0;
	AtomicCounter1 = 0;
	AtomicCounter2 = 0;
}

