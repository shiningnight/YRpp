/*
	Players
*/

#ifndef HOUSE_H
#define HOUSE_H

#include <HouseTypeClass.h>

//forward declarations
class SuperClass;
class BulletClass;
class BuildingClass;
class FactoryClass;
class TagClass;
class WaypointPathClass;
class WaypointClass;

//--- ScoreStruct - might as well have been a class, but I didn't find any proof for that.
struct ScoreStruct
{
	int Counts[0x200];
	int NumCounts;	//should maximally be 0x200 !!!
	int	ByteOrder;	//BYTEORDER_HOST or BYTEORDER_NETWORK
};
//-----

struct StartingTechnoStruct
{
	TechnoTypeClass *  Unit;
	CoordStruct *      CellCoords;
};

// that's how WW calls it, seems to track levels of how much it hates other houses... typical ww style, with bugs
struct AngerStruct
{
	HouseClass * House;
	int          AngerLevel;

	//need to define a == operator so it can be used in array classes
	bool operator == (AngerStruct tAnger)
	{ 
		return (House == tAnger.House &&
				AngerLevel == tAnger.AngerLevel);
	}
};

struct ScoutStruct
{
	HouseClass * House;
	bool         IsPreferred;

	//need to define a == operator so it can be used in array classes
	bool operator == (ScoutStruct tScout)
	{ 
		return (House == tScout.House &&
				IsPreferred == tScout.IsPreferred);
	}
};

//--- BaseNodeClass
class BaseNodeClass
{
public:
	 BaseNodeClass(){};
	~BaseNodeClass(){};

	//need to define a == operator so it can be used in array classes
	bool operator ==(BaseNodeClass tBaseNode)
	{
		return
			(BuildingTypeIndex == tBaseNode.get_BuildingTypeIndex()) &&
			(MapCoords == tBaseNode.get_MapCoords()) &&
			(unknown_8 == tBaseNode.get_unknown_8()) &&
			(unknown_C == tBaseNode.get_unknown_C());
	}

	PROPERTY(int,         BuildingTypeIndex);
	PROPERTY(CellStruct,  MapCoords);
	PROPERTY(DWORD,       unknown_8);
	PROPERTY(DWORD,       unknown_C);
};

//--- BaseClass - holds information about a player's base!
class HouseClass;	//forward declaration needed

class BaseClass
{	
public:
	 BaseClass() { THISCALL(0x42E6F0); };
	virtual ~BaseClass() { /*???*/ }; // gcc demands a virtual since virtual funcs exist

	PROTECTED_PROPERTY(void*, VirtualTable);

	//VTable
	virtual HRESULT _stdcall Load(IStream* pStm)
		{ PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x42EE30); }
	virtual HRESULT _stdcall Save(IStream* pStm)
		{ PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x42F070); }
	virtual void CalculateChecksum(void* pChkSum)
		{ PUSH_VAR32(pChkSum); THISCALL(0x42F180); }

	//Properties
	PROPERTY(DynamicVectorClass<BaseNodeClass>, BaseNodes);
	PROPERTY(int,                               PercentBuilt);
	PROPERTY(DynamicVectorClass<CellStruct>,    Cells_24);
	PROPERTY(DynamicVectorClass<CellStruct>,    Cells_38);
	PROPERTY(CellStruct,                        Center);

	PROTECTED_PROPERTY(BYTE,                    unknown_54[0x20]);

	PROPERTY(HouseClass*,                       Owner);
};

//--- Here we go, finally...
class HouseClass : public AbstractClass, public IHouse, public IPublicHouse, public IConnectionPointContainer
{
public:
	//Static
	static DynamicVectorClass<HouseClass*>* Array;

	static HouseClass* Player()
	{
		HouseClass* pPlayer;
		MEM_READ32(pPlayer, 0xA83D4C);
		return pPlayer;
	}

	//IConnectionPointContainer
	virtual HRESULT _stdcall EnumConnectionPoints(IEnumConnectionPoints** ppEnum)
		{ PUSH_VAR32(ppEnum); THISCALL(0x5024F0); }
  virtual HRESULT _stdcall FindConnectionPoint(GUID* riid, IConnectionPoint** ppCP)
		{ PUSH_VAR32(ppCP); PUSH_VAR32(riid); THISCALL(0x502550); }

	//IPublicHouse
	/* blargh
	virtual long _stdcall				ID_Number(){return IHouse::ID_Number();}
	virtual BSTR _stdcall				Name(){return IHouse::Name();}*/
  virtual long _stdcall Apparent_Category_Quantity(eCategory category)
		{ PUSH_VAR32(category); PUSH_VAR32(this); CALL(0x4F6A80); }
  virtual long _stdcall Apparent_Category_Power(eCategory category)
		{ PUSH_VAR32(category); PUSH_VAR32(this); CALL(0x4F6B50); }
  virtual CellStruct _stdcall Apparent_Base_Center()
  	{ PUSH_VAR32(this); CALL(0x4F6D10); }
	virtual bool _stdcall Is_Powered()
		{ PUSH_VAR32(this); CALL(0x4F6910); }

	//IHouse
	virtual long _stdcall ID_Number()           { return ArrayIndex;}
	virtual BSTR _stdcall Name()                { PUSH_VAR32(this); CALL(0x4F6950); }
	virtual IApplication* _stdcall	Get_Application() { PUSH_VAR32(this); CALL(0x4F6930); }
	virtual long _stdcall Available_Money()     { PUSH_VAR32(this); CALL(0x4F6990); }
	virtual long _stdcall Available_Storage()   { PUSH_VAR32(this); CALL(0x4F69D0); }
	virtual long _stdcall Power_Output()        { return PowerOutput;}
	virtual long _stdcall Power_Drain()         { return PowerDrain;}
  virtual long _stdcall Category_Quantity(eCategory category)
		{ PUSH_VAR32(category); PUSH_VAR32(this); CALL(0x4F6A20); }
  virtual long _stdcall Category_Power(eCategory category)
		{ PUSH_VAR32(category); PUSH_VAR32(this); CALL(0x4F6AE0); }
	virtual CellStruct _stdcall Base_Center()
		{ PUSH_VAR32(this); CALL(0x4F6BC0); }
	virtual HRESULT _stdcall Fire_Sale()
		{ PUSH_VAR32(this); CALL(0x5013A0); }
	virtual HRESULT _stdcall All_To_Hunt()
		{ PUSH_VAR32(this); CALL(0x501400); }

	//IUnknown
	virtual HRESULT _stdcall QueryInterface(REFIID iid, void** ppvObject)
		{ PUSH_VAR32(ppvObject); PUSH_VAR32(iid); PUSH_VAR32(this); CALL(0x4F6830); }

	virtual ULONG _stdcall AddRef()   { return 1; }
	virtual ULONG _stdcall Release()  { return 1; }

	//IPersist
	virtual HRESULT _stdcall GetClassID(CLSID* pClassID)
		{ PUSH_VAR32(pClassID); PUSH_VAR32(this); CALL(0x5046F0); }

	//IPersistStream
	virtual HRESULT _stdcall Load(IStream* pStm)
		{ PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x503040); }
	virtual HRESULT _stdcall Save(IStream* pStm, BOOL fClearDirty)
		{ PUSH_VAR32(fClearDirty); PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x504080); }

	//Destructor
	virtual ~HouseClass() { THISCALL(0x4F7140); }

	//AbstractClass
	virtual void PointerExpired(void* p, bool bUnknown)
		{ PUSH_VAR8(bUnknown); PUSH_VAR32(p); THISCALL(0x4FB9B0); }
	virtual eAbstractType		WhatAmI()
		{ return abs_House; }
	virtual int Size()
		{ return sizeof(HouseClass); }
	virtual void CalculateChecksum(void* pChkSum)
		{ PUSH_VAR32(pChkSum); THISCALL(0x502D60); }
	virtual int GetArrayIndex()
		{ return ArrayIndex; }
	virtual void Update()
		{ THISCALL(0x4F8440); }

	//Constructor
	HouseClass(HouseTypeClass* pCountry):AbstractClass(false)
  	{ PUSH_VAR32(pCountry); THISCALL(0x4F54A0); }

protected:
	HouseClass():AbstractClass(false){}
	HouseClass(bool X):AbstractClass(X){}

public:
	bool IsAlliedWith(int iHouse)
		{ PUSH_VAR32(iHouse); THISCALL(0x4F9A10); }
	bool IsAlliedWith(HouseClass* pOther)
		{ PUSH_VAR32(pOther); THISCALL(0x4F9A50); }
	bool IsAlliedWith(TechnoClass* pOther)
		{ PUSH_VAR32(pOther); THISCALL(0x4F9A90); }

	void MakeAlly(int iHouse, bool bAnnounce)
		{ PUSH_VAR8(bAnnounce); PUSH_VAR32(iHouse); THISCALL(0x4F9B50); }
	void MakeAlly(HouseClass* pWho, bool bAnnounce)
		{ PUSH_VAR8(bAnnounce); PUSH_VAR32(pWho); THISCALL(0x4F9B70); }
	void MakeEnemy(HouseClass* pWho, bool bAnnounce)
		{ PUSH_VAR8(bAnnounce); PUSH_VAR32(pWho); THISCALL(0x4F9F90); }

	void AllyAIHouses()
		{ THISCALL(0x501640); }

	// no explosions, just poooof
	void SDDTORAllAndTriggers()
		{ THISCALL(0x4FB920); }

	// every matching object takes damage and explodes
	void DestroyAll()
		{ THISCALL(0x4FC6D0); }
	void DestroyAllBuildings()
		{ THISCALL(0x4FC790); }
	void DestroyAllNonBuildingsNonNaval()
		{ THISCALL(0x4FC820); }
	void DestroyAllNonBuildingsNaval()
		{ THISCALL(0x4FC8D0); }

	void RespawnStartingBuildings()
		{ THISCALL(0x50D320); }
	void RespawnStartingForces()
		{ THISCALL(0x50D440); }

	byte Win(bool bSavourSomething)
		{ PUSH_VAR8(bSavourSomething); THISCALL(0x4FC9E0); }
	byte Lose(bool bSavourSomething)
		{ PUSH_VAR8(bSavourSomething); THISCALL(0x4FCBD0); }

	bool CanAlly(HouseClass* pOther)
		{ PUSH_VAR32(pOther); THISCALL(0x501540); }

	// warning: logic pretty much broken
	// pd: renamed to pTechnoType since Type is the HouseTypeClass*
	void LostPoweredCenter(TechnoTypeClass *pTechnoType)
		{ PUSH_VAR32(pTechnoType); THISCALL(0x50E0E0); } 
	void GainedPoweredCenter(TechnoTypeClass *pTechnoType)
		{ PUSH_VAR32(pTechnoType); THISCALL(0x50E1B0); }

	bool DoInfantrySelfHeal() { return this->InfantrySelfHeal > 0; }
	int  GetInfSelfHealStep() { THISCALL(0x50D9E0); }

	bool DoUnitsSelfHeal()     { return this->UnitsSelfHeal > 0; }
	int  GetUnitSelfHealStep() { THISCALL(0x50D9F0); }

	void CreatePowerOutage(int duration) { PUSH_VAR32(duration); THISCALL(0x50BC90); }
	void CreateRadarOutage(int duration) { PUSH_VAR32(duration); THISCALL(0x50BCD0); }
	// won't work if has spysat
	void ReshroudMap()  { THISCALL(0x50BD10); }

	void Cheer()        { THISCALL(0x50C8C0); }

	void BuildingUnderAttack(BuildingClass *pBld)
		{ PUSH_VAR32(pBld); THISCALL(0x4F93E0); }

	void TakeMoney(int amount) { PUSH_VAR32(amount); THISCALL(0x4F9790); }
	void GiveMoney(int amount) { PUSH_VAR32(amount); THISCALL(0x4F9950); }

	// no LostThreatNode() , this gets called also when node building dies! BUG
	void AcquiredThreatNode()
		{ THISCALL(0x509130); }

	// these are for mostly for map actions - HouseClass* foo = IsMP() ? Find_YesMP() : Find_NoMP();
	static bool Index_IsMP(int idx)
		{ SET_REG32(ecx, idx); CALL(0x510F60); }
	static HouseClass * FindByIndex_NoMP(int idxHouse)
		{ SET_REG32(ecx, idxHouse); CALL(0x50D230); }
	static HouseClass * FindByIndex_YesMP(int idxHouse)
		{ SET_REG32(ecx, idxHouse); CALL(0x510ED0); }

	CellStruct * PickIonCannonTarget(CellStruct &dest)
		{ PUSH_VAR32(dest); THISCALL(0x50CBF0); }

	WaypointClass * GetPlanningWaypointAt(CellStruct *coords)
		{ PUSH_VAR32(coords); THISCALL(0x5023B0); }
	bool GetPlanningWaypointProperties(WaypointClass *wpt, int &idxPath, byte &idxWP)
		{ PUSH_VAR8(idxWP); PUSH_VAR32(idxPath); PUSH_VAR32(wpt); THISCALL(0x502460); }

	// calls WaypointPathClass::WaypointPathClass() if needed
	void EnsurePlanningPathExists(int idx)
		{ PUSH_VAR32(idx); THISCALL(0x504740); }

	// finds a buildingtype from the given array that this house can currently build
	BuildingTypeClass* FirstBuildableFromArray(DynamicVectorClass<BuildingTypeClass*>* pArray)
		{ PUSH_VAR32(pArray); THISCALL(0x5051E0); }

	bool ControlledByHuman()
		{ THISCALL(0x50B730); }

	bool ControlledByPlayer()
		{ THISCALL(0x50B6F0); }

	// Target ought to be Object, I imagine, but cell doesn't work then
	void SendSpyPlanes(int AircraftTypeIdx, int AircraftAmount, eMission SetMission, AbstractClass *Target, ObjectClass *Destination)
		{ PUSH_VAR32(Destination); PUSH_VAR32(Target); PUSH_VAR32(SetMission); PUSH_VAR32(AircraftAmount); 
			SET_REG32(EDX, AircraftTypeIdx); THISCALL(0x65EAB0); }

	// registering in prereq counters (all technoes get logged, but only buildings get checked on validation... wtf)
	void RegisterGain(TechnoClass *pTechno, DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); PUSH_VAR32(pTechno); THISCALL(0x502A80); }

	void RegisterLoss(TechnoClass *pTechno, DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); PUSH_VAR32(pTechno); THISCALL(0x5025F0); }

	BuildingClass* FindBuildingOfType(int idx, signed int distance)
		{ PUSH_VAR32(distance); PUSH_VAR32(idx); THISCALL(0x4FD060); }

	AnimClass *PsiWarn(CellClass *pTarget, BulletClass *Bullet, char *AnimName)
		{ PUSH_VAR32(AnimName); PUSH_VAR32(Bullet); SET_REG32(EDX, pTarget); THISCALL(0x43B5E0); }

	// reminder: verify the resulting binary layout

	//===========================================================================
	//===== Properties ==========================================================
	//===========================================================================
	PROPERTY(int,                   ArrayIndex);
	PROPERTY(HouseTypeClass*,       Type);
	PROPERTY_STRUCT(DynamicVectorClass<TagClass*>,        RelatedTags);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   ConYards);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   Buildings);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   UnitRepairStations);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   Grinders);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   Absorbers);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   Bunkers);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   Occupiables);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   CloningVats);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   SecretLabs);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   PsychicDetectionBuildings);
	PROPERTY_STRUCT(DynamicVectorClass<BuildingClass*>,   FactoryPlants);
	PROPERTY(int,                   CountResourceGatherers);
	PROPERTY(int,                   CountResourceDestinations);
	PROPERTY(int,                   CountWarfactories);
	PROPERTY(int,                   InfantrySelfHeal);
	PROPERTY(int,                   UnitsSelfHeal);
	PROPERTY_STRUCT(DynamicVectorClass<StartingTechnoStruct*>,   StartingUnits);
	PROPERTY(int,                   AIDifficulty);
	PROPERTY(double,                FirepowerMultiplier); // used
	PROPERTY(double,                GroundspeedMultiplier); // unused ...
	PROPERTY(double,                AirspeedMultiplier);
	PROPERTY(double,                ArmorMultiplier);
	PROPERTY(double,                ROFMultiplier);
	PROPERTY(double,                CostMultiplier);
	PROPERTY(double,                BuildTimeMultiplier); // ... unused ends
	PROPERTY(double,                RepairDelay);
	PROPERTY(double,                BuildDelay);
	PROPERTY(int,                   IQLevel);
	PROPERTY(int,                   TechLevel);
	PROTECTED_PROPERTY(DWORD,       AltAllies); // ask question, receive brain damage
	PROPERTY(int,                   StartingCredits);	//not sure how these are used // actual credits = this * 100
	PROPERTY(eEdge,                  StartingEdge);		//??? use the EDGE macros
	PROPERTY(DWORD,                 unknown_1E4);
	PROPERTY(int,                   SideIndex);
	PROPERTY(bool,                  CurrentPlayer);		//is controlled by the player at this computer
	PROPERTY(bool,                  PlayerControl);		//a human controls this House
	PROPERTY(bool,                  Production);		//AI production has begun
	PROPERTY(bool,                  AutocreateAllowed);
	PROTECTED_PROPERTY(BYTE,        unknown_1EB[2]);
	PROPERTY(bool,                  AITriggersActive);
	PROPERTY(bool,                  AutoBaseBuilding);
	PROTECTED_PROPERTY(BYTE,        unknown_1EF[1]);
	PROPERTY(bool,                  Defeated);
	PROTECTED_PROPERTY(BYTE,        unknown_1D2[0x5]);
	PROPERTY(bool,                  HasThreatNode);
	PROPERTY(bool,                  ShouldRecheckTechTree);
	PROTECTED_PROPERTY(BYTE,        unknown_1F6[0x0F]);
	PROPERTY(int,                   SelectedPathIndex);
	PROPERTY_ARRAY(WaypointPathClass*, PlanningPaths, 0x0C); // 12 paths for "planning mode"
	PROPERTY(char,                  Visionary);			//??? exe says so
	PROPERTY(bool,                  MapIsClear);
	PROPERTY(bool,                  unknown_bool_242);
	PROPERTY(bool,                  unknown_bool_243);
	PROPERTY(bool,                  unknown_bool_244);
	PROPERTY(bool,                  Repairing); // BuildingClass::Repair, handholder for hurr durf AI
	PROPERTY(bool,                  unknown_bool_246);
	PROPERTY(bool,                  unknown_bool_247);
	PROPERTY(bool,                  unknown_bool_248);
	PROPERTY(bool,                  AllToHunt);
	PROPERTY(bool,                  unknown_bool_24A);
	PROPERTY(bool,                  unknown_bool_24B);
	PROPERTY(int,                   IQLevel2);			//no idea why we got this twice
	PROPERTY(eAIModes,              AIMode);
	PROPERTY_STRUCT(DynamicVectorClass<SuperClass*>,   Supers);
	PROPERTY(int,                   LastBuiltBuildingType);
	PROPERTY(int,                   LastBuiltInfantryType);
	PROPERTY(int,                   LastBuiltAircraftType);
	PROPERTY(int,                   LastBuiltVehicleType);
	PROPERTY(int,                   AllowWinBlocks); // some ra1 residue map trigger-fu, should die a painful death
	PROPERTY_STRUCT(TimerStruct,           RepairTimer); // for AI
	PROPERTY_STRUCT(TimerStruct,           unknown_timer_28C);
	PROPERTY_STRUCT(TimerStruct,           unknown_timer_298);
	PROPERTY_STRUCT(TimerStruct,           PowerBlackoutTimer);
	PROPERTY_STRUCT(TimerStruct,           RadarBlackoutTimer);
	PROPERTY(bool,                  Side2TechInfiltrated); // asswards! whether this player has infiltrated stuff
	PROPERTY(bool,                  Side1TechInfiltrated); // which is listed in [AI]->BuildTech
	PROPERTY(bool,                  Side0TechInfiltrated); // and has the appropriate AIBasePlanningSide
	PROPERTY(bool,                  BarracksInfiltrated);
	PROPERTY(bool,                  WarFactoryInfiltrated);

		// these four are unused horrors
		// checking prerequisites:
		/*
		if(1 << this->Country->IndexInArray & item->RequiredHouses
			|| (item->WhatAmI == abs_InfantryType && (item->RequiredHouses & this->InfantryAltOwner))
			|| (item->WhatAmI == abs_UnitType && (item->RequiredHouses & this->UnitAltOwner))
			|| (item->WhatAmI == abs_AircraftType && (item->RequiredHouses & this->AircraftAltOwner))
			|| (item->WhatAmI == abs_BuildingType && (item->RequiredHouses & this->BuildingAltOwner))
		)
			{ can build }
		 */
	PROPERTY(DWORD,					InfantryAltOwner);
	PROPERTY(DWORD,					UnitAltOwner);
	PROPERTY(DWORD,					AircraftAltOwner);
	PROPERTY(DWORD,					BuildingAltOwner);

	PROPERTY(int,                   AirportDocks);
	PROPERTY(int,                   PoweredUnitCenters);
	PROPERTY(DWORD,					unknown_2DC);
	PROPERTY(DWORD,					unknown_2E0);
	PROPERTY(DWORD,					unknown_2E4);
	PROPERTY(int,                   OwnedUnits);
	PROPERTY(int,                   OwnedNavy);
	PROPERTY(int,                   OwnedBuildings);
	PROPERTY(int,                   OwnedInfantry);
	PROPERTY(int,                   OwnedAircraft);
	PROPERTY_STRUCT(OwnedTiberiumStruct,	OwnedTiberium);
	PROPERTY(int,                   Balance);
	PROTECTED_PROPERTY(int,			Balance_);	//somehow connected to the Balance
	PROPERTY_STRUCT(OwnedTiberiumStruct,	unknown_OwnedTiberium_314);
	PROPERTY(DWORD,					unknown_324);
	PROPERTY_STRUCT(ScoreStruct,           BuiltInfantryTypes);
	PROPERTY_STRUCT(ScoreStruct,           BuiltUnitTypes);
	PROPERTY_STRUCT(ScoreStruct,           BuiltAircraftTypes);
	PROPERTY_STRUCT(ScoreStruct,           BuiltBuildingTypes);
	PROPERTY_STRUCT(ScoreStruct,           KilledInfantryTypes);
	PROPERTY_STRUCT(ScoreStruct,           KilledUnitTypes);
	PROPERTY_STRUCT(ScoreStruct,           KilledAircraftTypes);
	PROPERTY_STRUCT(ScoreStruct,           KilledBuildingTypes);
	PROPERTY_STRUCT(ScoreStruct,           UnknownScores);
	PROPERTY_STRUCT(ScoreStruct,           CollectedPowerups);	//YES, THIS IS HOW WW WASTES TONS OF RAM
	PROPERTY(int,                   NumAirpads);
	PROPERTY(int,                   NumBarracks);
	PROPERTY(int,                   NumWarFactories);
	PROPERTY(int,                   NumConYards);
	PROPERTY(int,                   NumShipyards);
	PROPERTY(int,                   NumOrePurifiers);
	PROPERTY(float,                 CostInfantryMult);
	PROPERTY(float,                 CostUnitsMult);
	PROPERTY(float,                 CostAircraftMult);
	PROPERTY(float,                 CostBuildingsMult);
	PROPERTY(float,                 CostDefensesMult);
	PROPERTY(int,                   PowerOutput);
	PROPERTY(int,                   PowerDrain);
	PROPERTY(FactoryClass*,         Primary_ForAircraft);
	PROPERTY(FactoryClass*,         Primary_ForInfantry);
	PROPERTY(FactoryClass*,         Primary_ForVehicles);
	PROPERTY(FactoryClass*,         Primary_ForShips);
	PROPERTY(FactoryClass*,         Primary_ForBuildings);
	PROPERTY(FactoryClass*,         Primary_Unused1);
	PROPERTY(FactoryClass*,         Primary_Unused2);
	PROPERTY(FactoryClass*,         Primary_Unused3);
	PROPERTY(FactoryClass*,         Primary_ForDefenses);
	PROPERTY(BYTE,					unknown_53D0);
	PROPERTY(BYTE,					unknown_53D1);
	PROPERTY(BYTE,					unknown_53D2);
	PROPERTY(BYTE,					unknown_53D3);
	PROPERTY(BYTE,					unknown_53D4);
	PROPERTY(BYTE,					unknown_53D5);
	PROPERTY(BYTE,					unknown_53D6);
	PROPERTY(BYTE,					unknown_53D7);
	PROPERTY(BYTE,					unknown_53D8);
	PROPERTY(BYTE,					unknown_53D9);
	PROPERTY(BYTE,					unknown_53DA);
	PROPERTY(BYTE,					unknown_53DB);
	PROPERTY(ObjectClass*,          OurFlagCarrier);
	PROPERTY_STRUCT(CellStruct,            OurFlagCoords);
	//for endgame score screen
	PROPERTY_ARRAY(int, KilledUnitsOfHouses, 0x14);		// 20 Houses only!
	PROPERTY(int,                   TotalKilledUnits);
	PROPERTY_ARRAY(int, KilledBuildingsOfHouses, 0x14);	// 20 Houses only!
	PROPERTY(int,                   TotalKilledBuildings);
	PROPERTY(DWORD,                 unknown_548C);
	PROPERTY(CellStruct*,           BaseSpawnCell);
	PROPERTY_STRUCT(CellStruct,            unknown_cell_5494);
	PROPERTY(DWORD,					unknown_5498);
	PROPERTY(DWORD,					unknown_549C);
	PROPERTY(DWORD,					unknown_54A0);
	PROPERTY(DWORD,					unknown_54A4);
	PROPERTY(DWORD,					unknown_54A8);
	PROPERTY(DWORD,					unknown_54AC);
	PROPERTY(DWORD,					unknown_54B0);
	PROPERTY(DWORD,					unknown_54B4);
	PROPERTY(DWORD,					unknown_54B8);
	PROPERTY(DWORD,					unknown_54BC);
	PROPERTY(DWORD,					unknown_54C0);
	PROPERTY(DWORD,					unknown_54C4);
	PROPERTY(DWORD,					unknown_54C8);
	PROPERTY(DWORD,					unknown_54CC);
	PROPERTY(DWORD,					unknown_54D0);
	PROPERTY(DWORD,					unknown_54D4);
	PROPERTY(DWORD,					unknown_54D8);
	PROPERTY(DWORD,					unknown_54DC);
	PROPERTY(DWORD,					unknown_54E0);
	PROPERTY(DWORD,					unknown_54E4);
	PROPERTY(int,                   SiloMoney);
	PROPERTY(int,                   PreferredTargetWaypoint); // set via map actions
	PROPERTY_STRUCT(CellStruct,            unknown_54F0);
	PROPERTY_STRUCT(CellStruct,            unknown_54F4);
	PROPERTY_STRUCT(CellStruct,            unknown_54F8);
	PROPERTY(int,                   PreferredDefensiveCellStartTime); // don't look at me...
	                                // map actions let you set an ai's ForceShield firing cell, this is related

		// Used for: Counting objects ever owned
		// altered on object gain only
		// BuildLimit < 0 validation uses this
	PROPERTY_STRUCT(CounterClass,          OwnedBuildingTypes);
	PROPERTY_STRUCT(CounterClass,          OwnedUnitTypes);
	PROPERTY_STRUCT(CounterClass,          OwnedInfantryTypes);
	PROPERTY_STRUCT(CounterClass,          OwnedAircraftTypes);

		// Used for: Counting objects currently owned
		// altered on each object's loss or gain
		// AITriggerType condition uses this
	PROPERTY_STRUCT(CounterClass,          OwnedBuildingTypes1);
	PROPERTY_STRUCT(CounterClass,          OwnedUnitTypes1);
	PROPERTY_STRUCT(CounterClass,          OwnedInfantryTypes1);
	PROPERTY_STRUCT(CounterClass,          OwnedAircraftTypes1);

		// Used for: Counting objects produced from Factory
		// not altered when things get taken over or removed
		// BuildLimit > 0 uses this
	PROPERTY_STRUCT(CounterClass,          OwnedBuildingTypes2);
	PROPERTY_STRUCT(CounterClass,          OwnedUnitTypes2);
	PROPERTY_STRUCT(CounterClass,          OwnedInfantryTypes2);
	PROPERTY_STRUCT(CounterClass,          OwnedAircraftTypes2);
	PROPERTY(DWORD,					unknown_55F0);
	PROPERTY(DWORD,					unknown_55F4);
	PROPERTY(int,                   AttackDelayA);
	PROPERTY(int,                   AttackDelayB); // both unused
	PROPERTY(int,                   EnemyHouseIndex);
	PROPERTY_STRUCT(DynamicVectorClass<AngerStruct>,   AngerNodes); //arghghghgh bugged
	PROPERTY_STRUCT(DynamicVectorClass<ScoutStruct>,   ScoutNodes); // filled with data which is never used, jood gob WW
	PROPERTY_STRUCT(TimerStruct,           unkTimer3);
	PROPERTY_STRUCT(TimerStruct,           unkTimer4);
	PROPERTY(int,                   ProducingBuildingTypeIndex);
	PROPERTY(int,                   ProducingUnitTypeIndex);
	PROPERTY(int,                   ProducingInfantryTypeIndex);
	PROPERTY(int,                   ProducingAircraftTypeIndex);
	PROPERTY(int,                   RatioAITriggerTeam);
	PROPERTY(int,                   RatioTeamAircraft);
	PROPERTY(int,                   RatioTeamInfantry);
	PROPERTY(int,                   RatioTeamBuildings);
	PROPERTY(int,                   BaseDefenseTeamCount);
	PROPERTY(DWORD,					unknown_5670);
	PROPERTY(DWORD,					unknown_5674);
	PROPERTY(DWORD,					unknown_5678);
	PROPERTY(DWORD,					unknown_567C);
	PROPERTY(DWORD,					unknown_5680);
	PROPERTY(DWORD,					unknown_5684);
	PROPERTY(DWORD,					unknown_5688);
	PROPERTY(DWORD,					unknown_568C);
	PROPERTY(DWORD,					unknown_5690);
	PROPERTY(DWORD,					unknown_5694);
	PROPERTY(DWORD,					unknown_5698);
	PROPERTY(DWORD,					unknown_569C);
	PROPERTY(DWORD,					unknown_56A0);
	PROPERTY(DWORD,					unknown_56A4);
	PROPERTY(DWORD,					unknown_56A8);
	PROPERTY(DWORD,					unknown_56AC);
	PROPERTY(DWORD,					unknown_56B0);
	PROPERTY(DWORD,					unknown_56B4);
	PROPERTY(DWORD,					unknown_56B8);
	PROPERTY(DWORD,					unknown_56BC);
	PROPERTY(DWORD,					unknown_56C0);
	PROPERTY(DWORD,					unknown_56C4);
	PROPERTY(DWORD,					unknown_56C8);
	PROPERTY(DWORD,					unknown_56CC);
	PROPERTY(DWORD,					unknown_56D0);
	PROPERTY(DWORD,					unknown_56D4);
	PROPERTY(DWORD,					unknown_56D8);
	PROPERTY(DWORD,					unknown_56DC);
	PROPERTY(DWORD,					unknown_56E0);
	PROPERTY(DWORD,					unknown_56E4);
	PROPERTY(DWORD,					unknown_56E8);
	PROPERTY(DWORD,					unknown_56EC);
	PROPERTY(DWORD,					unknown_56F0);
	PROPERTY(DWORD,					unknown_56F4);
	PROPERTY(byte,          HasCloakingRanges); // don't ask
	PROPERTY_STRUCT(ColorStruct,           Color);
	PROPERTY_STRUCT(ColorStruct,           Color2); // my idb says so
	PROPERTY(byte,          padding);
	PROPERTY_STRUCT(BaseClass,             Base);
	PROPERTY(bool,                  PowerBlackout);
	PROPERTY(bool,                  RadarBlackout);
	PROPERTY(bool,                  SpySatActive);
	PROPERTY(bool,                  IsBeingDrained);
	PROPERTY(eEdge,                   Edge); //??? use the EDGE macros
	PROPERTY_STRUCT(CellStruct,            EMPTarget);
	PROPERTY_STRUCT(CellStruct,            NukeTarget);
	PROPERTY(DWORD,                 Allies);	//flags, one bit per HouseClass instance
	                                        	//-> 32 players possible here
	PROPERTY_STRUCT(TimerStruct,           unknown_timer_5788);
	PROPERTY_STRUCT(TimerStruct,           TeamDelayTimer); // for AI attacks
	PROPERTY(DWORD,					unknown_57A0);
	PROPERTY(DWORD,					unknown_57A4);
	PROPERTY(DWORD,					unknown_57A8);
	PROPERTY(DWORD,					unknown_57AC);
	PROPERTY(DWORD,					unknown_57B0);
	PROPERTY(DWORD,					unknown_57B4);
	PROPERTY(DWORD,					unknown_57B8);
	PROPERTY(DWORD,					unknown_57BC);
	PROPERTY(DWORD,					unknown_57C0);
	PROPERTY(DWORD,					unknown_57C4);
	PROPERTY(DWORD,					unknown_57C8);
	PROPERTY(DWORD,					unknown_57CC);
	PROPERTY(DWORD,					unknown_57D0);
	PROPERTY(DWORD,					unknown_57D4);
	PROPERTY(DWORD,					unknown_57D8);
	PROPERTY(DWORD,					unknown_57DC);
	PROPERTY(DWORD,					unknown_57E0);
	
protected:
	unsigned int                    ThreatPosedEstimates[130][130]; // BLARGH

	PROPERTY_STRING(InitialName,		0x15);
	PROPERTY_UNICODE(NoIdeaButWillCheck,0x11);
	PROPERTY_UNICODE(UIName,			0x11);
	PROPERTY(int,                   ColorSchemeIndex);
	PROPERTY_STRUCT(CellStruct,            StartingCell);
	PROPERTY(DWORD,                 StartingAllies);
	PROPERTY(DWORD,                 unknown_16060);
	PROPERTY_STRUCT(DynamicVectorClass<IConnectionPoint*>,   WaypointPath);
	PROPERTY(DWORD,					unknown_1607C);
	PROPERTY(DWORD,					unknown_16080);
	PROPERTY(DWORD,					unknown_16084);
	PROPERTY(double,				unknown_double_16088);
	PROPERTY(double,				unknown_double_16090);
	PROPERTY(double,				unknown_double_16098);
	PROPERTY(float,					unknown_float_160A0);
	PROPERTY(float,					unknown_float_160A4);
	PROPERTY(int,					TotalOwnedInfantryCost);
	PROPERTY(int,                   TotalOwnedVehicleCost);
	PROPERTY(int,                   TotalOwnedAircraftCost);
	PROPERTY(DWORD,					unknown_160B4);
};

#endif
