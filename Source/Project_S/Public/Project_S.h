// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Project_S, Log, All);
#define PSLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PSLOG_S(Verbosity) UE_LOG(Project_S, Verbosity, TEXT("%s"), *PSLOG_CALLINFO)
#define PSLOG(Verbosity, Format, ...) UE_LOG(Project_S, Verbosity, TEXT("%s %s"), *PSLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define PSCHECK(Expr, ...) {if(!(Expr)) {PSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}