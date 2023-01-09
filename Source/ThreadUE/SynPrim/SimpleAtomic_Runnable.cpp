#include "SimpleAtomic_Runnable.h"
#include "ThreadUE/ThreadUEGameModeBase.h"

inline FSimpleAtomic_Runnable::FSimpleAtomic_Runnable(FColor Color, AThreadUEGameModeBase *OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool UseAtomic)
{
	GameMode_Ref = OwnerActor;
	NumberIteration = NeedIteration;
	bUseAtomicFlag = UseAtomic;
	bSeparateLogicFlag = SeparateLogic;

	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::FSimpleAtomic_Runnable"));
}

inline FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::~FSimpleAtomic_Runnabl"));
}

bool FSimpleAtomic_Runnable::Init()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Init"));
	return true;
}

uint32 FSimpleAtomic_Runnable::Run()
{
	//FPlatformProcess::Sleep(1.0f);
	
	for (int i = 0; i < NumberIteration; ++i)
	{
		if (bSeparateLogicFlag)
		{
			if (bUseAtomicFlag)
			{
				GameMode_Ref->AtomicCounter1.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NonAtomicCounter1);
			}
			else
			{
				GameMode_Ref->NonAtomicCounter1++;
			}
		}
		else
		{
			if (bUseAtomicFlag)
			{
				GameMode_Ref->AtomicCounter2.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NonAtomicCounter2);
			}
			else
			{
				GameMode_Ref->NonAtomicCounter2++;
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Run"));
	return 0;
}

void FSimpleAtomic_Runnable::Stop()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Stop"));
}

void FSimpleAtomic_Runnable::Exit()
{
	UE_LOG(LogTemp, Error, TEXT("FSimpleAtomic_Runnable::Exit"));
}
