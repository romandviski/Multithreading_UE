#pragma once

#include "CoreMinimal.h"

class AThreadUEGameModeBase;

class THREADUE_API FSimpleAtomic_Runnable : public FRunnable
{

public:
	FSimpleAtomic_Runnable(FColor Color, AThreadUEGameModeBase *OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool bIsAtomic);
	virtual ~FSimpleAtomic_Runnable() override;

	virtual  bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	int NumberIteration = 0;
	AThreadUEGameModeBase *GameMode_Ref = nullptr;
	bool bIsStopThread = false;
	bool bUseAtomicFlag = true;
	bool bSeparateLogicFlag = false;
};

