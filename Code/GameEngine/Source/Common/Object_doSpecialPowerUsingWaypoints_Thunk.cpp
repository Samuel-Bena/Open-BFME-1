// cl: /DNDEBUG /MD /EHsc
// Lift the Object::doSpecialPowerUsingWaypoints __emit thunk to clean C++.
//
// Three gates before the work happens: a non-zero mask at this+0x1A4 aborts
// outright, the action-manager check is skipped when the caller forces it, and
// the special-power module has to exist. Only then does the module get the
// waypoint and the command options through its vtable at +0x34.
//
// getSpecialPowerModule is the same callee the CommandButton::isReady analysis
// identified at 0x000401BF. The two-argument action-manager gate at 0x0002E4A1
// is BFME-only -- Zero Hour's canDoSpecialPower takes five arguments -- so its
// pinned name is inferred from this call site and marked as such in
// symbols.csv; the address is what matters for the rel32.

typedef unsigned int UnsignedInt;

class SpecialPowerTemplate;
class Waypoint;
class Object;

class SpecialPowerModuleInterface
{
public:
	virtual void unused00();
	virtual void unused01();
	virtual void unused02();
	virtual void unused03();
	virtual void unused04();
	virtual void unused05();
	virtual void unused06();
	virtual void unused07();
	virtual void unused08();
	virtual void unused09();
	virtual void unused10();
	virtual void unused11();
	virtual void unused12();
	virtual void doSpecialPowerUsingWaypoints(const Waypoint *, UnsignedInt);	///< vtable +0x34
};

class ActionManager
{
public:
	bool canDoSpecialPower(const Object *, const SpecialPowerTemplate *);		///< ILT thunk at 0x0002E4A1
};

extern ActionManager *TheActionManager;											///< retail [0x012ED80C]

class Object
{
public:
	void doSpecialPowerUsingWaypoints(const SpecialPowerTemplate *, const Waypoint *, UnsignedInt, bool);

	SpecialPowerModuleInterface *getSpecialPowerModule(const SpecialPowerTemplate *);	///< ILT thunk at 0x000401BF

private:
	unsigned char m_unreconstructed_00[0x1A4];
	UnsignedInt m_disabledMask;													///< retail this+0x1A4
};

// ?doSpecialPowerUsingWaypoints@Object@@QAEXPBVSpecialPowerTemplate@@PBVWaypoint@@I_N@Z
void Object::doSpecialPowerUsingWaypoints(const SpecialPowerTemplate *spTemplate, const Waypoint *way,
										  UnsignedInt commandOptions, bool forced)
{
	if (m_disabledMask)
		return;

	if (!forced && !TheActionManager->canDoSpecialPower(this, spTemplate))
		return;

	SpecialPowerModuleInterface *mod = getSpecialPowerModule(spTemplate);
	if (mod)
	{
		mod->doSpecialPowerUsingWaypoints(way, commandOptions);
	}
}
