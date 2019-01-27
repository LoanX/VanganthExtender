#include "StdAfx.h"

void CCreatureController::AsyncDoNothing()
{
	CExecutionArgument arg(CExecutionArgument::DoNothingArgument);
	asynchronousExecution.AddExecutable(this, CAsynchronousExecution::DoNothing, &arg);
}