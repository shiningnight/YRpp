/*
	Cells
*/

#ifndef CELL_H
#define CELL_H

#include <AbstractClass.h>

//forward declarations
class ObjectClass;
class TechnoClass;
class BuildingClass;
class BuildingTypeClass;
class UnitClass;
class InfantryClass;
class AircraftClass;
class TerrainClass;
class LightConvertClass;
class RadSiteClass;
class FootClass;

class CellClass : public AbstractClass
{
public:
	enum {AbsID = abs_Cell};

	//IPersist
	virtual HRESULT __stdcall GetClassID(CLSID* pClassID) R0;

	//IPersistStream
	virtual HRESULT __stdcall Load(IStream* pStm) R0;
	virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty) R0;

	//Destructor
	virtual ~CellClass() RX;

	//AbstractClass
	virtual eAbstractType WhatAmI() R0;
	virtual int Size() R0;

	// non-virtual

	// get content objects
	ObjectClass *FindObjectNearestTo(CoordStruct *coords, bool alt, ObjectClass *ExcludeThis)
		{ JMP_THIS(0x47C3D0); }

	ObjectClass *FindObjectOfType(int AbstractID, bool alt)
		{ JMP_THIS(0x47C4D0); }

	BuildingClass *GetBuilding()
		{ JMP_THIS(0x47C520); }

	UnitClass *GetUnit(bool alt)
		{ JMP_THIS(0x47EBA0); }

	InfantryClass *GetInfantry(bool alt)
		{ JMP_THIS(0x47EC40); }

	AircraftClass *GetAircraft(bool alt)
		{ JMP_THIS(0x47EBF0); }

	TerrainClass *GetTerrain(bool alt)
		{ JMP_THIS(0x47C550); }

	/* craziest thing... first iterates Content looking to Aircraft,
	 * failing that, calls FindObjectNearestTo,
	 * if that fails too, reiterates Content looking for Terrain
	 */
	ObjectClass *GetSomeObject(CoordStruct *coords, bool alt)
		{ JMP_THIS(0x47C5A0); }

	// misc
	void SetWallOwner()
		{ JMP_THIS(0x47D210); }

	// adjusts LAT
	void SetupLAT()
		{ JMP_THIS(0x47CA80); }

	// checks for nearby cliff impassability, calls SetupLAT(), sets up TubeClass if tunnel, cell anim if attached, etc
	void Setup(DWORD dwUnk)
		{ JMP_THIS(0x47D2B0); }

	void BlowUpBridge()
		{ JMP_THIS(0x47DD70); }

	bool CanThisExistHere(eSpeedType SpeedType, BuildingTypeClass *Object, HouseClass* Owner)
		{ JMP_THIS(0x47C620); }

	// those unks are passed to TechnoClass::Scatter in that same order
	void ScatterContent(DWORD unk2, DWORD unk3, DWORD unk4, bool alt)
		{ JMP_THIS(0x481670); }

	CellClass *GetNeighbourCell(unsigned int direction)
		{ JMP_THIS(0x481810); }

	// called whenever anything moves, first to remove threat from source cell, second time to add threat to dest cell
	void UpdateThreat(unsigned int SourceHouse, int ThreatLevel)
		{ JMP_THIS(0x481870); }

	void CollectCrate(FootClass *Collector)
		{ JMP_THIS(0x481A00); }

	void ProcessColourComponents(int *arg0, int *Intensity, int *Ambient, int *a5, int *a6, int *tintR, int *tintG, int *tintB)
		{ JMP_THIS(0x484180); }

	/*TubeClass*/void GetTunnel()
		{ JMP_THIS(0x484F20); }

	RectangleStruct *GetContainingRect(RectangleStruct *dest)
		{ JMP_THIS(0x47FB90); }

	// don't laugh, it returns the uiname of contained tiberium... which nobody ever sets
	wchar_t *GetUIName()
		{ JMP_THIS(0x484FF0); }

	// returns the tiberium's index in OverlayTypes
	int GetContainedTiberiumIndex()
		{ JMP_THIS(0x485010); }

	int GetContainedTiberiumValue()
		{ JMP_THIS(0x485020); }

	int SetMapCoords(CoordStruct *coords)
		{ JMP_THIS(0x485240); }

	// in leptons
	CoordStruct *Get3DCoords(CoordStruct *result)
		{ JMP_THIS(0x486840); }

	// depends on one of the cell flags being set
	CoordStruct *Get3DCoords2(CoordStruct *result)
		{ JMP_THIS(0x486890); }

	// used by ambient waves and stuff
	CoordStruct *Get3DCoords3(CoordStruct *result)
		{ JMP_THIS(0x480A30); }

	void ActivateVeins()
		{ JMP_THIS(0x486920); }

	// cloak generators
	bool CloakGen_InclHouse(unsigned int idx)
		{ return ((1 << idx) & this->CloakedByHouses) != 0; }

	void CloakGen_AddHouse(unsigned int idx)
		{ this->CloakedByHouses |= 1 << idx; }

	void CloakGen_RemHouse(unsigned int idx)
		{ this->CloakedByHouses &= ~(1 << idx); }

	// unused, returns 0 if that house doesn't have cloakgens covering this cell or Player has sensors over this cell
	bool DrawObjectsCloaked(int OwnerHouseIdx)
		{ JMP_THIS(0x486800); }

	// sensors
	bool Sensors_InclHouse(unsigned int idx)
		{ return this->SensorsOfHouses[idx] > 0; }

	void Sensors_AddOfHouse(unsigned int idx)
		{ ++this->SensorsOfHouses[idx]; }

	void Sensors_RemOfHouse(unsigned int idx)
		{ --this->SensorsOfHouses[idx]; }

	// disguise sensors
	bool DisguiseSensors_InclHouse(unsigned int idx)
		{ return this->DisguiseSensorsOfHouses[idx] > 0; }

	void DisguiseSensors_AddOfHouse(unsigned int idx)
		{ ++this->DisguiseSensorsOfHouses[idx]; }

	void DisguiseSensors_RemOfHouse(unsigned int idx)
		{ --this->DisguiseSensorsOfHouses[idx]; }

	// Rad Sites
	void SetRadSite(RadSiteClass *Rad)
		{ this->RadSite = Rad; }

	RadSiteClass* GetRadSite()
		{ return this->RadSite; }

	bool IsRadiated()
		{ JMP_THIS(0x487C90); }

	int GetRadLevel()
		{ JMP_THIS(0x487CB0); }

	void RadLevel_Increase(double amount)
		{ JMP_THIS(0x487CE0); }

	void RadLevel_Decrease(double amount)
		{ JMP_THIS(0x487D00); }

	// helper
	bool ContainsBridge()
		{ return (this->Flags & cf_Bridge) != 0; }
	// helper mimicking game's behaviour
	ObjectClass* GetContent()
		{ return this->ContainsBridge() ? this->AltObject : this->FirstObject; }

	// tilesets
#define ISTILE(tileset, addr) \
	bool Tile_Is_ ## tileset() \
		{ JMP_THIS(addr); }

	ISTILE(Tunnel, 0x484AB0);
	ISTILE(Water, 0x485060);
	ISTILE(Blank, 0x486380);
	ISTILE(Ramp, 0x4863A0);
	ISTILE(Cliff, 0x4863D0);
	ISTILE(Shore, 0x4865B0);
	ISTILE(Wet, 0x4865D0);
	ISTILE(MiscPave, 0x486650);
	ISTILE(Pave, 0x486670);
	ISTILE(DirtRoad, 0x486690);
	ISTILE(PavedRoad, 0x4866D0);
	ISTILE(PavedRoadEnd, 0x4866F0);
	ISTILE(PavedRoadSlope, 0x486710);
	ISTILE(Median, 0x486730);
	ISTILE(Bridge, 0x486750);
	ISTILE(WoodBridge, 0x486770);
	ISTILE(ClearToSandLAT, 0x486790);
	ISTILE(Green, 0x4867B0);
	ISTILE(NotWater, 0x4867E0);
	ISTILE(DestroyableCliff, 0x486900);

	// HACK EVIL HACK
	static int BridgeHeight()
	{ return *(int *)0xB0C07C; }

	static CoordStruct * Cell2Coord(CellStruct* cell, CoordStruct *crd)
	{
		crd->X = cell->X * 256 + 128;
		crd->Y = cell->Y * 256 + 128;
		crd->Z = 0;
		return crd;
	}

	static CellStruct * Coord2Cell(CoordStruct *crd, CellStruct* cell)
	{
		cell->X = short(crd->X / 256);
		cell->Y = short(crd->Y / 256);
		return cell;
	}

	CoordStruct * FixHeight(CoordStruct *crd)
	{
		crd->Z += ((this->Flags & cf_Bridge) != 0) * BridgeHeight();
		return crd;
	}

	// helper - gets coords and fixes height for bridge
	CoordStruct * GetCoordsWithBridge(CoordStruct *crd)
	{
		this->GetCoords(crd);
		return this->FixHeight(crd);
	}

	CoordStruct * FindInfantrySubposition(CoordStruct *dst, CoordStruct *src, char bUnk1, char bUnk2, char bUnk3)
		{ JMP_THIS(0x481180); }

	bool TryAssignJumpjet(FootClass *Object)
		{ JMP_THIS(0x487D70); }

protected:
	//Constructor
	CellClass() : AbstractClass(false)
		{ JMP_THIS(0x47BBF0); }

	//===========================================================================
	//===== Properties ==========================================================
	//===========================================================================

public:

	CellStruct MapCoords;	//Where on the map does this Cell lie?

	PROTECTED_PROPERTY(BYTE,     unknown_28[0xC]);

	LightConvertClass* LightConvert;
	int                IsoTileTypeIndex;	//What tile is this Cell?

	PROTECTED_PROPERTY(BYTE,     unknown_3C[8]);

	int                OverlayTypeIndex;	//What Overlay lies on this Cell?
	int                SmudgeTypeIndex;	//What Smudge lies on this Cell?

	DWORD              unknown_4C;
	int                WallOwnerIndex; // Which House owns the wall placed in this Cell?
	                                              // Determined by finding the nearest BuildingType and taking its owner
	DWORD              unknown_54;
	DWORD              unknown_58;
	DWORD              unknown_5C;
	DWORD              unknown_60;
	DWORD              unknown_64;
	DWORD              unknown_68;
	DWORD              unknown_6C;
	DWORD              unknown_70;
	DWORD              unknown_74;
	DWORD              CloakedByHouses;	//Is this cell in a cloak generator's radius? One bit per House.

	// Is this cell in range of some SensorsSight= equipment? One Word(!) per House, ++ and -- per unit.
protected:
	unsigned short               SensorsOfHouses[0x18]; // ! 24 houses instead of 32 like cloakgen
	// use Sensors_ funcs above

	// Is this cell in range of some DetectDisguise= equipment? One Word(!) per House, ++ and -- per unit.
protected:
	unsigned short               DisguiseSensorsOfHouses[0x18]; // ! 24 houses instead of 32 like cloakgen
	// use DisguiseSensors_ funcs above

public:

	DWORD              BaseSpacerOfHouses; // & (1 << HouseX->ArrayIndex) == base spacing dummy for HouseX
	FootClass *        Jumpjet; // a jumpjet occupying this cell atm

	ObjectClass*       FirstObject;	//The first Object on this Cell. NextObject functions as a linked list.

	ObjectClass*       AltObject;

//	PROTECTED_PROPERTY(BYTE,	unknown_E8[4]);

	eLandType          LandType;	//What type of floor is this Cell?
	double             RadLevel;	//The level of radiation on this Cell.
	RadSiteClass*      RadSite;	//A pointer to the responsible RadSite.

	DWORD              unknown_FC;
	int                OccupyHeightsCoveringMe;
	DWORD              unknown_104;
	WORD               unknown_108;
	WORD               unknown_10A;
	WORD               unknown_10C;
	WORD               unknown_10E;
	WORD               unknown_110;
	WORD               unknown_112;
	WORD               unknown_114;
	signed short       TubeIndex; // !@#% Westwood braindamage, can't use > 127! (movsx eax, al)

	char               unknown_118;
	char               unknown_119;
	char               Height;
	char               Level;

	BYTE               SlopeIndex;  // this + 2 == cell's slope shape as reflected by PLACE.SHP
	PROTECTED_PROPERTY(BYTE,     unknown_11D);

	unsigned char      Powerup;	//The crate type on this cell. Also indicates some other weird properties

	PROTECTED_PROPERTY(BYTE,     unknown_11F);
	BYTE               Shroudedness; // trust me, you don't wanna know... if you do, see 0x7F4194 and cry
	BYTE               unknown_121[0xB];

	eCellFlags_12C     CopyFlags;	// related to Flags below
	DWORD              IsUnderShroud; // only 0 or 1, but modified as an integer
	DWORD              GapsCoveringThisCell; // actual count of gapgens in this cell, no idea why they need a second layer
	PROTECTED_PROPERTY(BYTE,     unknown_138[0x4]);
	DWORD              unknown_13C;

	eCellFlags         Flags;	//Various settings.
	PROTECTED_PROPERTY(BYTE,     unknown_144[4]);
};

#endif
