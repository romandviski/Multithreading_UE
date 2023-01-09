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

	StopSimpleCounterThread();
}

// SimpleCounter here
void AThreadUEGameModeBase::CreateSimpleCounterThread()
{
	if (!CurrentRunningGameModeThread_SimpleCounter) // проверяем что потока нет
		{
		if (!MyRunnableClass_SimpleCounter)
		{
			MyRunnableClass_SimpleCounter = new FSimpleCounter_Runnable(ColorSimpleCounter, this, bIsUseSafeVariable);
		}
		// создаём поток
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread"), 0, EThreadPriority::TPri_Normal);
		}
}

bool AThreadUEGameModeBase::SwitchRunStateSimpleCounterThread(bool bIsPause)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(bIsPause);
	}

	return !bIsPause;
}

void AThreadUEGameModeBase::StopSimpleCounterThread()
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter)
		{
			CurrentRunningGameModeThread_SimpleCounter->Suspend(false);
			// Not safe bool
			MyRunnableClass_SimpleCounter->bIsStopThread = true;
			// Safe bool
			MyRunnableClass_SimpleCounter->bIsStopThreadSafe = true;

			// интересные штуки:
			//FThreadSingletonInitializer
			//FMemStack - выделение памяти
			//FThreadStats - статистика
			//FFakeThread - фейковый поток
			//FPlatformProcess::SupportsMultithreading() - есть ли поддержка мультипотока

			CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion(); // ждём завершения
		
			//delete CurrentRunningGameModeThread_SimpleCounter; // логично, но в анриле свой сборщик
			CurrentRunningGameModeThread_SimpleCounter = nullptr;
			MyRunnableClass_SimpleCounter = nullptr;
		}
	}
}

void AThreadUEGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(false);
		CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait); // убиваем поток
		CurrentRunningGameModeThread_SimpleCounter = nullptr;
		MyRunnableClass_SimpleCounter = nullptr;
	}
}

int64 AThreadUEGameModeBase::GetSimpleCounterThread()
{
	int64 result = -1;
	if (MyRunnableClass_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter->bIsUseSafeVariable)
		{
			result = MyRunnableClass_SimpleCounter->CounterSafe.GetValue();
		}
		else
		{
			result = MyRunnableClass_SimpleCounter->Counter;
		}
	}
	return result;
}


// SimpleAtomic here
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
