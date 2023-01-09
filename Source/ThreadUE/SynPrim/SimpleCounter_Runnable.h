#pragma once

class AThreadUEGameModeBase;

class THREADUE_API FSimpleCounter_Runnable : public FRunnable
{

public:
	FSimpleCounter_Runnable(FColor Color, AThreadUEGameModeBase *OwnerActor, bool VariableMode);
	virtual ~FSimpleCounter_Runnable() override;

	// Safe variable // атомарные переменные
	FThreadSafeBool bIsStopThreadSafe = FThreadSafeBool(false); // заполнение через конструктор
	FThreadSafeCounter CounterSafe = FThreadSafeCounter(0);
	
	// Not save variable
	bool bIsStopThread = false;
	int64 Counter = 0;

	// settings
	AThreadUEGameModeBase *GameMode_Ref = nullptr;
	bool bIsUseSafeVariable = false;
	
	virtual  bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	
};
