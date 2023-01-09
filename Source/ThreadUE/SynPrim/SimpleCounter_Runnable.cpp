#include "SimpleCounter_Runnable.h"
#include "ThreadUE/ThreadUEGameModeBase.h"

FSimpleCounter_Runnable::FSimpleCounter_Runnable(FColor Color, AThreadUEGameModeBase* OwnerActor, bool VariableMode)
{
	GameMode_Ref = OwnerActor;
	bIsUseSafeVariable = VariableMode;
}

FSimpleCounter_Runnable::~FSimpleCounter_Runnable()
{
	
}

bool FSimpleCounter_Runnable::Init()
{
	return true;
}

#pragma optimize("", off) // из-за оптимизации компилятора, может не срабатывать булевая
uint32 FSimpleCounter_Runnable::Run()
{
	if (bIsUseSafeVariable)
	{
		while (!bIsStopThreadSafe)
		{
			CounterSafe.Increment();
		}
	}
	else
	{
		while (!bIsStopThread) // косяк с булевой
		{
			Counter++;
			//FPlatformProcess::Sleep(0.02f); // иногда виноват компилятор, можно замедлить
		}	
	}

	return 1;
}
#pragma optimize("", on)

void FSimpleCounter_Runnable::Stop()
{
	FRunnable::Stop();

	bIsStopThread = true;
	bIsStopThreadSafe = true;
}

void FSimpleCounter_Runnable::Exit()
{
	FRunnable::Exit();
	GameMode_Ref = nullptr;
}
