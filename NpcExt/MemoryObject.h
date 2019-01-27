#pragma once

class CObject;
class CItem;
class CCreature;
class User;
class CSocket;

class MemoryObject
{
public:
/* 0008 */  int	nRefCount;
/* 000C */  int	nIdentity; //unique id
/* 0010 */  int	nInTimer;
/* 0014 */  int	nDeleteCount;

/* 0000 */  virtual void          Release(const char* file, int line, int nWorkType, bool flag) = 0;
/* 0008 */  virtual MemoryObject* AddRef(const char* file, int line, int nWorkType) = 0;
/* 0010 */  virtual bool        _vfunc010() = 0;
/* 0018 */  virtual             ~MemoryObject() {}

/* 0020 */  virtual bool        IsObject() = 0;
/* 0028 */  virtual bool        IsUser() = 0;
/* 0030 */  virtual bool        IsItem() = 0;
/* 0038 */  virtual bool        IsCreature() = 0;

/* 0040 */  virtual CObject*    _AsObject() = 0;
/* 0048 */  virtual CItem*      _CastItem() = 0;
/* 0050 */  virtual CCreature*  _CastCreature() = 0;
/* 0058 */  virtual User*       _CastUser() = 0;

/* 0060 */  virtual void        ReleaseError() = 0;
/* 0068 */  virtual int         RefCount() = 0;
/* 0070 */  virtual void        ReportOrt(const char* file, int line, int nWorkType) = 0;
};