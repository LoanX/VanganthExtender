#pragma once 

static const DWORD  VT_MEMORYOBJECT		=	0x97E3B8;
static const DWORD  VT_COBJECT			=	0xA37138;
static const DWORD  VT_CIOOBJECT		=	0xA94FA8;
static const DWORD  VT_CGENERALOBJECT	=	0xA383A8;
static const DWORD  VT_CSTATICOBJECT	=	0xA75888;
static const DWORD  VT_CVEHICLE			=	0xA9A068;
static const DWORD  VT_CCREATURE		=	0x9B08B8;
static const DWORD  VT_CNPC				=	0xA289D8;
static const DWORD  VT_CSUMMON			=	0xA4A708;
static const DWORD  VT_CPET				=	0xA4B078;
static const DWORD  VT_CBOSS			=	0x988F78;
static const DWORD  VT_CMERCHANT		=	0xA1ABB8;
static const DWORD  VT_CZZOLDAGU		=	0xAA09C8;
static const DWORD  VT_USER				=	0xA871B8;
static const DWORD	VT_PARTY			=	0xA40418;
static const DWORD  VT_ITEM				=	0xA07CF8;
static const DWORD  VT_ITEMWEAPON		=	0xA0B368;
static const DWORD  VT_ITEMETC			=	0xA0A448;
static const DWORD  VT_ITEMASSET		=	0xA09558;
static const DWORD  VT_ITEMARMOR		=	0xA09CC8;
static const DWORD  VT_ITEMACCESSARY	=	0xA08DC8;
static const DWORD  VT_ITEMQUEST		=	0xA0ABD8;
static const DWORD  VT_CSIGNBOARD		=	0xA5EE98;
static const DWORD	VT_CCASTLE			=	0x9A4038;
static const DWORD	VT_CBATTLECAMP		=	0x987E88;
static const DWORD	VT_CRESIDENCE		=	0xA57EE8;
static const DWORD	VT_CAGIT			=	0x976EF8;
static const DWORD	VT_CAUCTIONAGIT		=	0x987568;
static const DWORD	VT_CNPCBATTLEAGIT	=	0xA2B0E8;
static const DWORD	VT_CSIEGEAGIT		=	0xA5DF88;
static const DWORD	VT_CTEAMBATTLEAGIT	=	0xA779B8;
static const DWORD	VT_CDOOR			=	0x9E20F8;

class MemoryObject
{
public:
/* 0008 */  int	nRefCount;
/* 000C */  int	nIdentity; 
/* 0010 */  int	nInTimer;
/* 0014 */  int	nDeleteCount;

/* 0000 */  virtual void          Release(const char* file, int line, int nWorkType, bool flag) = 0;
/* 0008 */  virtual MemoryObject* AddRef(const char* file, int line, int nWorkType) = 0;
/* 0010 */  virtual bool        _vfunc010() = 0;
/* 0018 */  virtual             ~MemoryObject() {}

/* 0020 */  virtual bool        _IsObject() = 0;
/* 0028 */  virtual bool        _IsUser() = 0;
/* 0030 */  virtual bool        _IsItem() = 0;
/* 0038 */  virtual bool        _IsCreature() = 0;

/* 0040 */  virtual CObject*    _AsObject() = 0;
/* 0048 */  virtual CItem*      _CastItem() = 0;
/* 0050 */  virtual CCreature*  _CastCreature() = 0;
/* 0058 */  virtual User*       _CastUser() = 0;

/* 0060 */  virtual void        ReleaseError() = 0;
/* 0068 */  virtual int         RefCount() = 0;
/* 0070 */  virtual void        ReportOrt(const char* file, int line, int nWorkType) = 0;
/*			MemoryObject()
			{
				typedef MemoryObject*(*f)(MemoryObject*);
				f(0x6F2360L)(this);
			}
*/
};
