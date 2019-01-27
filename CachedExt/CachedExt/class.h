#pragma once

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include "math.h"
#include "time.h"

using namespace std;

class MemoryObject;
class CSocket;
class CQuerySocket;
class CPledge;
class CUser;
class CItem;
class CWareHouse;