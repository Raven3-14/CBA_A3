/*
	Header: script_macros_common.hpp
	
	Description:
		A general set of useful macro functions for use by CBA itself or by any module that uses CBA.

	Authors:
		Sickboy <sb_at_dev-heaven.net> and Spooner
*/

/* ****************************************************
 New - Should be exported to general addon
 Aim:
   - Simplify (shorten) the amount of characters required for repetitive tasks
   - Provide a solid structure that can be dynamic and easy editable (Which sometimes means we cannot adhere to Aim #1 ;-)
     An example is the path that is built from defines. Some available in this file, others in mods and addons.
 
 Follows  Standard:
   Object variables: PREFIX_COMPONENT
   Main-object variables: PREFIX_main
   Paths: MAINPREFIX\PREFIX\SUBPREFIX\COMPONENT\SCRIPTNAME.sqf
   e.g: x\six\addons\sys_menu\fDate.sqf
 
 Usage:
   define PREFIX and COMPONENT, then include this file
   (Note, you could have a main addon for your mod, define the PREFIX in a macros.hpp,
   and include this script_macros_common.hpp file.
   Then in your addons, add a component.hpp, define the COMPONENT,
   and include your mod's script_macros.hpp
   In your scripts you can then include the addon's component.hpp with relative path)
 
 TODO:
   - Try only to use 1 string type " vs '
   - Evaluate double functions, and simplification
   - Evaluate naming scheme; current = prototype
   - Evaluate "Debug" features..
   - Evaluate "create mini function per precompiled script, that will load the script on first usage, rather than on init"
   - Also saw "Namespace" typeName, evaluate which we need :P
   - Single/Multi player gamelogics? (Incase of MP, you would want only 1 gamelogic per component, which is pv'ed from server, etc)
 */

#ifndef MAINPREFIX
	#define MAINPREFIX x
#endif

#ifndef SUBPREFIX
	#define SUBPREFIX addons
#endif

#ifndef MAINLOGIC
	#define MAINLOGIC main
#endif

#ifndef VERSION
	#define VERSION 0
#endif

#ifndef VERSION_AR
	#define VERSION_AR VERSION
#endif

#ifndef VERSION_CONFIG
	#define VERSION_CONFIG version = VERSION; versionStr = QUOTE(VERSION); versionAr[] = {VERSION_AR}
#endif

/* -------------------------------------------
Group: Debugging
------------------------------------------- */

/* -------------------------------------------
Macros: DEBUG_MODE_x
	Managing debugging based on debug level.

	According to the *highest* level of debugging that has been defined *before* script_macros_common.hpp is included,
	only the appropriate debugging commands will be functional. With no level explicitely defined, assume DEBUG_MODE_NORMAL.
	
	DEBUG_MODE_FULL - Full debugging output.
	DEBUG_MODE_NORMAL - All debugging except <TRACE_n()> and <LOG()> (Default setting if none specified).
	DEBUG_MODE_MINIMAL - Only <ERROR()> and <ERROR_WITH_TITLE()> enabled.
	
Examples:
	In order to turn on full debugging for a single file,
	(begin example)
		// Top of individual script file.
		#define DEBUG_MODE_FULL
		#include "script_component.hpp"
	(end)
	
	In order to force minimal debugging for a single component,
	(begin example)
		// Top of addons\<component>\script_component.hpp
		// Ensure that any FULL and NORMAL setting from the individual files are undefined and MINIMAL is set.
		#ifdef DEBUG_MODE_FULL
		#undef DEBUG_MODE_FULL
		#endif
		#ifdef DEBUG_MODE_NORMAL
		#undef DEBUG_MODE_NORMAL
		#endif
		#ifndef DEBUG_MODE_MINIMAL
		#define DEBUG_MODE_MINIMAL
		#endif
		#include "script_macros.hpp"
	(end)
	
	In order to turn on full debugging for a whole addon,
	(begin example)
		// Top of addons\main\script_macros.hpp
		#ifndef DEBUG_MODE_FULL
		#define DEBUG_MODE_FULL
		#endif
		#include "\x\cba\addons\main\script_macros_common.hpp"
	(end)
	
Author:
	Spooner
------------------------------------------- */

// If DEBUG_MODE_FULL, then also enable DEBUG_MODE_NORMAL.
#ifdef DEBUG_MODE_FULL
#define DEBUG_MODE_NORMAL
#endif

// If DEBUG_MODE_NORMAL, then also enable DEBUG_MODE_MINIMAL.
#ifdef DEBUG_MODE_NORMAL
#define DEBUG_MODE_MINIMAL
#endif

// If no debug modes specified, use DEBUG_MODE_NORMAL (+ DEBUG_MODE_MINIMAL).
#ifndef DEBUG_MODE_MINIMAL
#define DEBUG_MODE_NORMAL
#define DEBUG_MODE_MINIMAL
#endif

#ifdef THIS_FILE
#define THIS_FILE_ 'THIS_FILE'
#else
#define THIS_FILE_ __FILE__
#endif

/* -------------------------------------------
Macro: LOG()
	Log a timestamped message into the RPT log.

	Only run if <DEBUG_MODE_FULL> or higher is defined.
	
Parameters:
	MESSAGE - Message to record [String]
	
Example:
	(begin example)
		LOG("Initiated clog-dancing simulator.");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#ifdef DEBUG_MODE_FULL
#define LOG(MESSAGE) [THIS_FILE_, __LINE__, MESSAGE] call CBA_fnc_log
#else
#define LOG(MESSAGE) /* disabled */
#endif

/* -------------------------------------------
Macro: WARNING()
	Record a timestamped, non-critical error in the RPT log.

	Only run if <DEBUG_MODE_NORMAL> or higher is defined.
	
Parameters:
	MESSAGE - Message to record [String]
	
Example:
	(begin example)
		WARNING("This function has been deprecated. Please don't use it in future!");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#ifdef DEBUG_MODE_NORMAL
#define WARNING(MESSAGE) [THIS_FILE_, __LINE__, ('WARNING: ' + MESSAGE)] call CBA_fnc_log
#else
#define WARNING(MESSAGE) /* disabled */
#endif

/* -------------------------------------------
Macro: ERROR()
	Record a timestamped, critical error in the RPT log. 
	
	The heading is "ERROR" (use <ERROR_WITH_TITLE()> for a specific title).

	TODO: Popup an error dialog & throw an exception.
	
Parameters:
	MESSAGE -  Message to record [String]
	
Example:
	(begin example)
		ERROR("Value not found","value of frog not found in config ...yada...yada...");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ERROR(MESSAGE) \
	[THIS_FILE_, __LINE__, "ERROR", MESSAGE] call CBA_fnc_error;
	
/* -------------------------------------------
Macro: ERROR_WITH_TITLE()
	Record a timestamped, critical error in the RPT log.
	
	The title can be specified (in <ERROR()> the heading is always just "ERROR")
	Newlines (\n) in the MESSAGE will be put on separate lines.

	TODO: Popup an error dialog & throw an exception.
	
Parameters:
	TITLE - Title of error message [String]
	MESSAGE -  Body of error message [String]
	
Example:
	(begin example)
		ERROR_WITH_TITLE("Value not found","Value of frog not found in config ...yada...yada...");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ERROR_WITH_TITLE(TITLE,MESSAGE) \
	[THIS_FILE_, __LINE__, TITLE, MESSAGE] call CBA_fnc_error;

/* -------------------------------------------
Macro: RETNIL()
	If a variable is undefined, return the value nil. Otherwise, return the
	variable itself.

Parameters:
	VARIABLE - the variable to check

Example:
	(begin example)
		// _var is undefined
		hintSilent format ["_var=%1", RETNIL(_var) ]; // "_var=any"
	(end example)

Author:
	Alef (see CBA issue #8514)
------------------------------------------- */
#define RETNIL(VARIABLE) if (isNil{VARIABLE}) then {nil} else {VARIABLE}

/* -------------------------------------------
Macros: TRACE_n()
	Log a message and 1-8 variables to the RPT log.

	Only run if <DEBUG_MODE_FULL> is defined.

	TRACE_1(MESSAGE,A) - Log 1 variable.
	TRACE_2(MESSAGE,A,B) - Log 2 variables.
	TRACE_3(MESSAGE,A,B,C) - Log 3 variables.
	TRACE_4(MESSAGE,A,B,C,D) - Log 4 variables.
	TRACE_5(MESSAGE,A,B,C,D,E) - Log 5 variables.
	TRACE_6(MESSAGE,A,B,C,D,E,F) - Log 6 variables.
	TRACE_7(MESSAGE,A,B,C,D,E,F,G) - Log 7 variables.
	TRACE_8(MESSAGE,A,B,C,D,E,F,G,H) - Log 8 variables.

Parameters:
	MESSAGE -  Message to add to the trace [String]
	A..H - Variable names to log values of [Any]

Example:
	(begin example)
		TRACE_3("After takeoff",_vehicle player,getPos (_vehicle player), getPosASL (_vehicle player));
	(end)
	
Author:
	Spooner
------------------------------------------- */
#ifdef DEBUG_MODE_FULL
#define TRACE_1(MESSAGE,A) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2', MESSAGE, RETNIL(A)]] call CBA_fnc_log
	
#define TRACE_2(MESSAGE,A,B) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3', MESSAGE, RETNIL(A), RETNIL(B)]] call CBA_fnc_log
	
#define TRACE_3(MESSAGE,A,B,C) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C)]] call CBA_fnc_log
	
#define TRACE_4(MESSAGE,A,B,C,D) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4, D=%5', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C), RETNIL(D)]] call CBA_fnc_log
	
#define TRACE_5(MESSAGE,A,B,C,D,E) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4, D=%5, E=%6', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C), RETNIL(D), RETNIL(E)]] call CBA_fnc_log
	
#define TRACE_6(MESSAGE,A,B,C,D,E,F) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4, D=%5, E=%6, F=%7', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C), RETNIL(D), RETNIL(E), RETNIL(F)]] call CBA_fnc_log
	
#define TRACE_7(MESSAGE,A,B,C,D,E,F,G) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4, D=%5, E=%6, F=%7, G=%8', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C), RETNIL(D), RETNIL(E), RETNIL(F), RETNIL(G)]] call CBA_fnc_log
	
#define TRACE_8(MESSAGE,A,B,C,D,E,F,G,H) \
	[THIS_FILE_, __LINE__, format ['%1: A=%2, B=%3, C=%4, D=%5, E=%6, F=%7, G=%8, H=%9', MESSAGE, RETNIL(A), RETNIL(B), RETNIL(C), RETNIL(D), RETNIL(E), RETNIL(F), RETNIL(G), RETNIL(H)]] call CBA_fnc_log

#else

#define TRACE_1(MESSAGE,A) /* disabled */
#define TRACE_2(MESSAGE,A,B) /* disabled */
#define TRACE_3(MESSAGE,A,B,C) /* disabled */
#define TRACE_4(MESSAGE,A,B,C,D) /* disabled */
#define TRACE_5(MESSAGE,A,B,C,D,E) /* disabled */
#define TRACE_6(MESSAGE,A,B,C,D,E,F) /* disabled */
#define TRACE_7(MESSAGE,A,B,C,D,E,F,G) /* disabled */
#define TRACE_8(MESSAGE,A,B,C,D,E,F,G,H) /* disabled */

#endif

/* -------------------------------------------
Group: General
------------------------------------------- */

// *************************************
// Internal Functions
#define DOUBLES(var1,var2) ##var1##_##var2
#define TRIPLES(var1,var2,var3) ##var1##_##var2##_##var3
#define QUOTE(var1) #var1

#ifdef MODULAR
	#define COMPONENT_T DOUBLES(t,COMPONENT)
	#define COMPONENT_M DOUBLES(m,COMPONENT)
	#define COMPONENT_S DOUBLES(s,COMPONENT)
	#define COMPONENT_C DOUBLES(c,COMPONENT)
	#define COMPONENT_F COMPONENT_C
#else
	#define COMPONENT_T COMPONENT
	#define COMPONENT_M COMPONENT
	#define COMPONENT_S COMPONENT
	#define COMPONENT_F COMPONENT
	#define COMPONENT_C COMPONENT
#endif

/* -------------------------------------------
Macro: INC()

Description:
	Increase a number by one.
	
Parameters:
	VAR - Variable to increment [Number]
	
Example:
	(begin example)
	_counter = 0;
	INC(_counter);
	// _counter => 1
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define INC(var) var = (var) + 1

/* -------------------------------------------
Macro: DEC()

Description:
	Decrease a number by one.
	
Parameters:
	VAR - Variable to decrement [Number]
	
Example:
	(begin example)
	_counter = 99;
	DEC(_counter);
	// _counter => 98
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define DEC(var) var = (var) - 1

/* -------------------------------------------
Macro: ADD()

Description:
	Add a value to a variable. Variable and value should be both Numbers or both Strings.

Parameters:
	VAR - Variable to add to [Number or String]
	VALUE - Value to add [Number or String]
	
Examples:
	(begin example)
		_counter = 2;
		ADD(_counter,3);
		// _counter => 5
	(end)
	(begin example)
		_str = "hello";
		ADD(_str," ");
		ADD(_str,"Fred");
		// _str => "hello Fred"
	(end)
	
Author:
	Sickboy
------------------------------------------- */
#define ADD(var1,var2) var1 = (var1) + (var2)

/* -------------------------------------------
Macro: SUB()

Description:
	Subtract a value from a number variable. VAR and VALUE should both be Numbers.

Parameters:
	VAR - Variable to subtract from [Number]
	VALUE - Value to subtract [Number]
	
Examples:
	(begin example)
		_numChickens = 2;
		SUB(_numChickens,3);
		// _numChickens => -1
	(end)
------------------------------------------- */
#define SUB(var1,var2) var1 = (var1) - (var2)

/* -------------------------------------------
Macro: REM()

Description:
	Remove an element from an array each time it occurs.
	
	This recreates the entire array, so use BIS_fnc_removeIndex if modification of the original array is required
	or if only one of the elements that matches ELEMENT needs to be removed.

Parameters:
	ARRAY - Array to modify [Array]
	ELEMENT - Element to remove [Any]
	
Examples:
	(begin example)
		_array = [1, 2, 3, 4, 3, 8];
		REM(_array,3);
		// _array = [1, 2, 4, 8];
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define REM(var1,var2) SUB(var1,[var2])

/* -------------------------------------------
Macro: PUSH()

Description:
	Appends a single value onto the end of an ARRAY. Change is made to the ARRAY itself, not creating a new array.

Parameters:
	ARRAY - Array to push element onto [Array]
	ELEMENT - Element to push [Any]
	
Examples:
	(begin example)
		_fish = ["blue", "green", "smelly"];
		PUSH(_fish,"monkey-flavoured");
		// _fish => ["blue", "green", "smelly", "monkey-flavoured"]
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define PUSH(var1,var2) var1 set [count (var1), var2]

/* -------------------------------------------
Macro: ISNILS()

Description:
	Sets a variable with a value, but only if it is undefined.

Parameters:
	VARIABLE - Variable to set [Any, not nil]
	DEFAULT_VALUE - Value to set VARIABLE to if it is undefined [Any, not nil]
	
Examples:
	(begin example)
		// _fish is undefined
		ISNILS(_fish,0);
		// _fish => 0
	(end)
	(begin example)
		_fish = 12;
		// ...later...
		ISNILS(_fish,0);
		// _fish => 12
	(end)
	
Author:
	Sickboy
------------------------------------------- */
#define ISNILS(VARIABLE,DEFAULT_VALUE) if (isNil #VARIABLE) then { ##VARIABLE = ##DEFAULT_VALUE }
#define ISNILS2(var1,var2,var3,var4) ISNILS(TRIPLES(var1,var2,var3),var4)
#define ISNILS3(var1,var2,var3) ISNILS(DOUBLES(var1,var2),var3)
#define ISNIL(var1,var2) ISNILS2(PREFIX,COMPONENT,var1,var2)
#define ISNILMAIN(var1,var2) ISNILS3(PREFIX,var1,var2)

#define CREATELOGICS(var1,var2) ##var1##_##var2## = ([sideLogic] call CBA_fnc_getSharedGroup) createUnit ["LOGIC", [0, 0, 0], [], 0, "NONE"]
#define CREATELOGICLOCALS(var1,var2) ##var1##_##var2## = "LOGIC" createVehicleLocal [0, 0, 0]
#define CREATELOGICGLOBALS(var1,var2) ##var1##_##var2## = ([sideLogic] call CBA_fnc_getSharedGroup) createUnit ["LOGIC", [0, 0, 0], [], 0, "NONE"]; publicVariable QUOTE(DOUBLES(var1,var2))
#define CREATELOGICGLOBALTESTS(var1,var2) ##var1##_##var2## = ([sideLogic] call CBA_fnc_getSharedGroup) createUnit [QUOTE(TRIPLES(PREFIX,COMPONENT,logic)), [0, 0, 0], [], 0, "NONE"]

#define GETVARS(var1,var2,var3) (##var1##_##var2 getVariable #var3)
#define GETVARMAINS(var1,var2) GETVARS(var1,MAINLOGIC,var2)

#define PATHTO_SYS(var1,var2,var3) MAINPREFIX\##var1\SUBPREFIX\##var2\##var3.sqf
#define PATHTOF_SYS(var1,var2,var3) \MAINPREFIX\##var1\SUBPREFIX\##var2\##var3
#define PATHTOF2_SYS(var1,var2,var3) MAINPREFIX\##var1\SUBPREFIX\##var2\##var3

#define PATHTO_R(var1) PATHTOF2_SYS(PREFIX,COMPONENT_C,var1)
#define PATHTO_T(var1) PATHTOF_SYS(PREFIX,COMPONENT_T,var1)
#define PATHTO_M(var1) PATHTOF_SYS(PREFIX,COMPONENT_M,var1)
#define PATHTO_S(var1) PATHTOF_SYS(PREFIX,COMPONENT_S,var1)
#define PATHTO_C(var1) PATHTOF_SYS(PREFIX,COMPONENT_C,var1)
#define PATHTO_F(var1) PATHTO_SYS(PREFIX,COMPONENT_F,var1)

#define COMPILE_FILE_SYS(var1,var2,var3) compile preProcessFileLineNumbers 'PATHTO_SYS(var1,var2,var3)'

#define SETVARS(var1,var2) ##var1##_##var2 setVariable
#define SETVARMAINS(var1) SETVARS(var1,MAINLOGIC)
#define GVARS(var1,var2,var3) ##var1##_##var2##_##var3
#define GVARMAINS(var1,var2) ##var1##_##var2##
#define CFGSETTINGSS(var1,var2) configFile >> "CfgSettings" >> #var1 >> #var2
//#define SETGVARS(var1,var2,var3) ##var1##_##var2##_##var3 = 
//#define SETGVARMAINS(var1,var2) ##var1##_##var2 = 

#define PREPMAIN_SYS(var1,var2,var3) ##var1##_fnc_##var3 = { ##var1##_fnc_##var3 = COMPILE_FILE_SYS(var1,var2,DOUBLES(fnc,var3)); if (isNil "_this") then { call ##var1##_fnc_##var3 } else { _this call ##var1##_fnc_##var3 } }

// Compile-Once, JIT: On first use.
// #define PREP_SYS(var1,var2,var3) ##var1##_##var2##_fnc_##var3 = { ##var1##_##var2##_fnc_##var3 = COMPILE_FILE_SYS(var1,var2,DOUBLES(fnc,var3)); if (isNil "_this") then { call ##var1##_##var2##_fnc_##var3 } else { _this call ##var1##_##var2##_fnc_##var3 } }
// #define PREP_SYS2(var1,var2,var3,var4) ##var1##_##var2##_fnc_##var4 = { ##var1##_##var2##_fnc_##var4 = COMPILE_FILE_SYS(var1,var3,DOUBLES(fnc,var4)); if (isNil "_this") then { call ##var1##_##var2##_fnc_##var4 } else { _this call ##var1##_##var2##_fnc_##var4 } }

// Compile-Once, at Macro. As opposed to Compile-Once, on first use.
#define PREP_SYS(var1,var2,var3) ##var1##_##var2##_fnc_##var3 = COMPILE_FILE_SYS(var1,var2,DOUBLES(fnc,var3))
#define PREP_SYS2(var1,var2,var3,var4) ##var1##_##var2##_fnc_##var4 = COMPILE_FILE_SYS(var1,var3,DOUBLES(fnc,var4))

#define LSTR(var1) TRIPLES(ADDON,STR,var1)


#ifndef DEBUG_SETTINGS
	#define DEBUG_SETTINGS [false, true, false]
#endif

#define MSG_INIT QUOTE(Initializing: ADDON version: VERSION)

// *************************************
// User Functions
// Please define PREFIX and COMPONENT before including
#define ADDON DOUBLES(PREFIX,COMPONENT)
#define MAIN_ADDON DOUBLES(PREFIX,main)

#define CFGSETTINGS CFGSETTINGSS(PREFIX,COMPONENT)
#define PATHTO(var1) PATHTO_SYS(PREFIX,COMPONENT_F,var1)
#define PATHTOF(var1) PATHTOF_SYS(PREFIX,COMPONENT,var1)

#define COMPILE_FILE(var1) COMPILE_FILE_SYS(PREFIX,COMPONENT_F,var1)


#define VERSIONING_SYS(var1) class CfgSettings \
{ \
	class CBA \
	{ \
		class Versioning \
		{ \
			class var1 \
			{ \
			}; \
		}; \
	}; \
};

#define VERSIONING VERSIONING_SYS(PREFIX)

/* -------------------------------------------
Macro: GVAR()
	Get full variable identifier for a global variable owned by this component.
	
Parameters:
	VARIABLE - Partial name of global variable owned by this component [Any].

Example:
	(begin example)
		GVAR(frog) = 12;
		// In SPON_FrogDancing component, equivalent to SPON_FrogDancing_frog = 12
	(end)

Author:
	Sickboy
------------------------------------------- */
#define GVAR(var1) GVARS(PREFIX,COMPONENT,var1)

/* -------------------------------------------
Macro: GVARMAIN()
	Get full variable identifier for a global variable owned by this addon.
	
Parameters:
	VARIABLE - Partial name of global variable owned by this addon [Any].

Example:
	(begin example)
		GVARMAIN(frog) = 12;
		// In SPON_FrogDancing component, equivalent to SPON_frog = 12
	(end)

Author:
	Sickboy
------------------------------------------- */
#define GVARMAIN(var1) GVARMAINS(PREFIX,var1)
// TODO: What's this?
#define SETTINGS DOUBLES(PREFIX,settings)
#define CREATELOGIC CREATELOGICS(PREFIX,COMPONENT)
#define CREATELOGICGLOBAL CREATELOGICGLOBALS(PREFIX,COMPONENT)
#define CREATELOGICGLOBALTEST CREATELOGICGLOBALTESTS(PREFIX,COMPONENT)
#define CREATELOGICLOCAL CREATELOGICLOCALS(PREFIX,COMPONENT)
#define CREATELOGICMAIN CREATELOGICS(PREFIX,MAINLOGIC)
#define GETVAR(var1) GETVARS(PREFIX,COMPONENT,var1)
#define SETVAR SETVARS(PREFIX,COMPONENT)
#define SETVARMAIN SETVARMAINS(PREFIX)
#define IFCOUNT(var1,var2,var3) if (count ##var1 > ##var2) then { ##var3 = ##var1 select ##var2 };

//#define PREP(var1) PREP_SYS(PREFIX,COMPONENT_F,var1)
#define PREP(var1) PREP_SYS2(PREFIX,COMPONENT,COMPONENT_F,var1)
#define PREPMAIN(var1) PREPMAIN_SYS(PREFIX,COMPONENT_F,var1)
#define FUNC(var1) TRIPLES(DOUBLES(PREFIX,COMPONENT),fnc,var1)
#define FUNCMAIN(var1) TRIPLES(PREFIX,fnc,var1)
#define FUNC_INNER(var1,var2) TRIPLES(DOUBLES(PREFIX,var1),fnc,var2)

#define ARG_1(A,B) ((A) select (B))
#define ARG_2(A,B,C) (ARG_1(ARG_1(A,B),C))
#define ARG_3(A,B,C,D) (ARG_1(ARG_2(A,B,C),D))
#define ARG_4(A,B,C,D,E) (ARG_1(ARG_3(A,B,C,D),E))
#define ARG_5(A,B,C,D,E,F) (ARG_1(ARG_4(A,B,C,D,E),F))
#define ARG_6(A,B,C,D,E,F,G) (ARG_1(ARG_5(A,B,C,D,E,F),G))
#define ARG_7(A,B,C,D,E,F,G,H) (ARG_1(ARG_6(A,B,C,D,E,E,F,G),H))
#define ARG_8(A,B,C,D,E,F,G,H,I) (ARG_1(ARG_7(A,B,C,D,E,E,F,G,H),I))

// CONTROL(46) 12
#define DISPLAY(A) (findDisplay A)
#define CONTROL(A) DISPLAY(A) displayCtrl

/* -------------------------------------------
Macros: IS_x()
	Checking the data types of variables.

	IS_ARRAY() - Array
	IS_BOOL() - Boolean
	IS_BOOLEAN() - UI display handle(synonym for <IS_BOOL()>)
	IS_CODE() - Code block (i.e a compiled function)
	IS_CONFIG() - Configuration
	IS_CONTROL() - UI control handle.
	IS_DISPLAY() - UI display handle.
	IS_FUNCTION() - A compiled function (synonym for <IS_CODE()>)
	IS_GROUP() - Group.
	IS_INTEGER() - Is a number a whole number?
	IS_LOCATION() - World location.
	IS_NUMBER() - A floating point number (synonym for <IS_SCALAR()>)
	IS_OBJECT() - World object.
	IS_SCALAR() - Floating point number.
	IS_SCRIPT() - A script handle (as returned by execVM and spawn commands).
	IS_SIDE() - Game side.
	IS_STRING() - World object.
	IS_TEXT() - Structured text.
	
Parameters:
	VARIABLE - Variable to check if it is of a particular type [Any, not nil]
	
Author:
	Spooner
------------------------------------------- */
#define IS_ARRAY(VAR)    ((typeName (VAR)) == "ARRAY")
#define IS_BOOL(VAR)     ((typeName (VAR)) == "BOOL")
#define IS_CODE(VAR)     ((typeName (VAR)) == "CODE")
#define IS_CONFIG(VAR)   ((typeName (VAR)) == "CONFIG")
#define IS_CONTROL(VAR)  ((typeName (VAR)) == "CONTROL")
#define IS_DISPLAY(VAR)  ((typeName (VAR)) == "DISPLAY")
#define IS_GROUP(VAR)    ((typeName (VAR)) == "GROUP")
#define IS_OBJECT(VAR)   ((typeName (VAR)) == "OBJECT")
#define IS_SCALAR(VAR)   ((typeName (VAR)) == "SCALAR")
#define IS_SCRIPT(VAR)   ((typeName (VAR)) == "SCRIPT")
#define IS_SIDE(VAR)     ((typeName (VAR)) == "SIDE")
#define IS_STRING(VAR)   ((typeName (VAR)) == "STRING")
#define IS_TEXT(VAR)     ((typeName (VAR)) == "TEXT")
#define IS_LOCATION(VAR) ((typeName (VAR)) == "LOCATION")

#define IS_BOOLEAN(VAR)  IS_BOOL(VAR)
#define IS_FUNCTION(VAR) IS_CODE(VAR)
#define IS_INTEGER(VAR)  if { IS_SCALAR(VAR) } then { (floor(VAR) == (VAR)) } else { false }
#define IS_NUMBER(VAR)   IS_SCALAR(VAR)

/* -------------------------------------------
Macro: SCRIPT()
	Sets name of script (relies on PREFIX and COMPONENT values being #defined).
	
Parameters:
	NAME - Name of script [Indentifier]
	
Example:
	(begin example)
		SCRIPT(eradicateMuppets);
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define SCRIPT(NAME) \
	scriptName 'PREFIX\COMPONENT\NAME'
	
/* -------------------------------------------
Macros: EXPLODE_n()
	Splitting an ARRAY into a number of variables (A, B, C, etc).
	
	Note that this does not make the created variables private.

	EXPLODE_2(ARRAY,A,B) - Split a 2-element array into separate variables.
	EXPLODE_3(ARRAY,A,B,C) - Split a 3-element array into separate variables.
	EXPLODE_4(ARRAY,A,B,C,D) - Split a 4-element array into separate variables.
	EXPLODE_5(ARRAY,A,B,C,D,E) - Split a 5-element array into separate variables.
	EXPLODE_6(ARRAY,A,B,C,D,E,F) - Split a 6-element array into separate variables.
	EXPLODE_7(ARRAY,A,B,C,D,E,F,G) - Split a 7-element array into separate variables.
	EXPLODE_8(ARRAY,A,B,C,D,E,F,G,H) - Split a 8-element array into separate variables.
	
Parameters:
	ARRAY - Array to read from [Array]
	A..H - Names of variables to set from array [Identifier]
	
Example:
	(begin example)
		_array = ["fred", 156.8, 120.9];
		EXPLODE_3(_array,_name_height,_weight);
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define EXPLODE_2(ARRAY,A,B) \
	A = (ARRAY) select 0; B = (ARRAY) select 1
	
#define EXPLODE_3(ARRAY,A,B,C) \
	EXPLODE_2(ARRAY,A,B); C = (ARRAY) select 2
	
#define EXPLODE_4(ARRAY,A,B,C,D) \
	EXPLODE_3(ARRAY,A,B,C); D = (ARRAY) select 3
	
#define EXPLODE_5(ARRAY,A,B,C,D,E) \
	EXPLODE_4(ARRAY,A,B,C,D); E = (ARRAY) select 4
	
#define EXPLODE_6(ARRAY,A,B,C,D,E,F) \
	EXPLODE_5(ARRAY,A,B,C,D,E); F = (ARRAY) select 5
	
#define EXPLODE_7(ARRAY,A,B,C,D,E,F,G) \
	EXPLODE_6(ARRAY,A,B,C,D,E,F); G = (ARRAY) select 6
	
#define EXPLODE_8(ARRAY,A,B,C,D,E,F,G,H) \
	EXPLODE_7(ARRAY,A,B,C,D,E,F,G); H = (ARRAY) select 7

/* -------------------------------------------
Group: Managing Function Parameters
------------------------------------------- */

/* -------------------------------------------
Macros: PARAMS_n()
	Setting variables based on parameters passed to a function.
	
	Each parameter is defines as private and set to the appropriate value from _this.

	PARAMS_1(A) - Get 1 parameter from the _this array (or _this if it's not an array).
	PARAMS_2(A,B) - Get 2 parameters from the _this array.
	PARAMS_3(A,B,C) - Get 3 parameters from the _this array.
	PARAMS_4(A,B,C,D) - Get 4 parameters from the _this array.
	PARAMS_5(A,B,C,D,E) - Get 5 parameters from the _this array.
	PARAMS_6(A,B,C,D,E,F) - Get 6 parameters from the _this array.
	PARAMS_7(A,B,C,D,E,F,G) - Get 7 parameters from the _this array.
	PARAMS_8(A,B,C,D,E,F,G,H) - Get 8 parameters from the _this array.

Parameters:
	A..H - Name of variable to read from _this [Identifier]
	
Example:
	A function called like this:
	(begin example)
		[_name,_address,_telephone] call recordPersonalDetails;
	(end)
	expects 3 parameters and those variables could be initialised at the start of the function definition with:
	(begin example)
		recordPersonalDetails = {
			PARAMS_3(_name,_address,_telephone);
			// Rest of function follows...
		};
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define PARAMS_1(A) \
	private #A; \
	A = if (IS_ARRAY(_this)) then {_this select 0} else {_this}; \
	TRACE_1("PARAMS_1",A)
	
#define PARAMS_2(A,B) \
	private [#A, #B]; \
	EXPLODE_2(_this,A,B); \
	TRACE_2("PARAMS_2",A,B)
	
#define PARAMS_3(A,B,C) \
	private [#A, #B, #C]; \
	EXPLODE_3(_this,A,B,C); \
	TRACE_3("PARAMS_3",A,B,C)
	
#define PARAMS_4(A,B,C,D) \
	private [#A, #B, #C, #D]; \
	EXPLODE_4(_this,A,B,C,D); \
	TRACE_4("PARAMS_4",A,B,C,D)
	
#define PARAMS_5(A,B,C,D,E) \
	private [#A, #B, #C, #D, #E]; \
	EXPLODE_5(_this,A,B,C,D,E); \
	TRACE_5("PARAMS_5",A,B,C,D,E)
	
#define PARAMS_6(A,B,C,D,E,F) \
	private [#A, #B, #C, #D, #E, #F]; \
	EXPLODE_6(_this,A,B,C,D,E,F); \
	TRACE_6("PARAMS_6",A,B,C,D,E,F)
	
#define PARAMS_7(A,B,C,D,E,F,G) \
	private [#A, #B, #C, #D, #E, #F, #G]; \
	EXPLODE_7(_this,A,B,C,D,E,F,G); \
	TRACE_7("PARAMS_7",A,B,C,D,E,F,G)
	
#define PARAMS_8(A,B,C,D,E,F,G,H) \
	private [#A, #B, #C, #D, #E, #F, #G, #H]; \
	EXPLODE_8(_this,A,B,C,D,E,F,G,H); \
	TRACE_8("PARAMS_8",A,B,C,D,E,F,G,H)
	
/* -------------------------------------------
Macro: DEFAULT_PARAM() 
	Getting a default function parameter. This may be used together with <PARAMS_n()> to have a mix of required and
	optional parameters.

Parameters:
	INDEX - Index of parameter in _this [Integer, 0+]
	NAME - Name of the variable to set [Identifier]
	DEF_VALUE - Default value to use in case the array is too short or the value at INDEX is nil [Any]
	
Example:
	A function called with optional parameters:
	(begin example)
		[_name] call myFunction;
		[_name, _numberOfLegs] call myFunction;
		[_name, _numberOfLegs, _hasAHead] call myFunction;
	(end)
	1 required parameter and 2 optional parameters. Those variables could be initialised at the start of the function 
	definition with:
	(begin example)
		myFunction = {
			PARAMS_1(_name);
			DEFAULT_PARAM(1,_numberOfLegs,2);
			DEFAULT_PARAM(2,_hasAHead,true);
			// Rest of function follows...
		};
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define DEFAULT_PARAM(INDEX,NAME,DEF_VALUE) \
	private #NAME; \
	NAME = [RETNIL(_this), INDEX, DEF_VALUE] call CBA_fnc_defaultParam; \
	TRACE_3("DEFAULT_PARAM",INDEX,NAME,DEF_VALUE)

/* -------------------------------------------
Group: Assertions
------------------------------------------- */

#define ASSERTION_ERROR(MESSAGE) ERROR_WITH_TITLE("Assertion failed!",MESSAGE)

/* -------------------------------------------
Macro: ASSERT_TRUE()
	Asserts that a CONDITION is true. When an assertion fails, an error is raised with the given MESSAGE.

Parameters:
	CONDITION - Condition to assert as true [Boolean]
	MESSSAGE - Message to display if (A OPERATOR B) is false [String]
	
Example:
	(begin example)
		ASSERT_TRUE(_frogIsDead,"The frog is alive");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ASSERT_TRUE(CONDITION,MESSAGE) \
	if (not (CONDITION)) then \
	{ \
		ASSERTION_ERROR('Assertion (CONDITION) failed!\n\n' + (MESSAGE)); \
	}

/* -------------------------------------------
Macro: ASSERT_FALSE()
	Asserts that a CONDITION is false. When an assertion fails, an error is raised with the given MESSAGE.

Parameters:
	CONDITION - Condition to assert as false [Boolean]
	MESSSAGE - Message to display if (A OPERATOR B) is true [String]
	
Example:
	(begin example)
		 ASSERT_FALSE(_frogIsDead,"The frog died");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ASSERT_FALSE(CONDITION,MESSAGE) \
	if (CONDITION) then \
	{ \
		ASSERTION_ERROR('Assertion (not (CONDITION)) failed!\n\n' + (MESSAGE)) \
	}

/* -------------------------------------------
Macro: ASSERT_OP()
	Asserts that (A OPERATOR B) is true. When an assertion fails, an error is raised with the given MESSAGE.

Parameters:
	A - First value [Any]
	OPERATOR - Binary operator to use [Operator]
	B - Second value [Any]
	MESSSAGE - Message to display if (A OPERATOR B)  is false. [String]
	
Example:
	(begin example)
		ASSERT_OP(_fish,>,5,"Too few fish!");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ASSERT_OP(A,OPERATOR,B,MESSAGE) \
	if (not ((A) OPERATOR (B))) then \
	{ \
		ASSERTION_ERROR('Assertion (A OPERATOR B) failed!\n' + 'A: ' + (str (A)) + '\n' + 'B: ' + (str (B)) + "\n\n" + (MESSAGE)); \
	}

/* -------------------------------------------
Macro: ASSERT_DEFINED()
	Asserts that a VARIABLE is defined. When an assertion fails, an error is raised with the given MESSAGE..

Parameters:
	VARIABLE - Variable to test if defined [String or Function].
	MESSAGE - Message to display if variable is undefined [String].
	
Examples:
	(begin example)
		ASSERT_DEFINED("_anUndefinedVar","Too few fish!");
		ASSERT_DEFINED({ obj getVariable "anUndefinedVar" },"Too many fish!");
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define ASSERT_DEFINED(VARIABLE,MESSAGE) \
	if (isNil VARIABLE) then \
	{ \
		ASSERTION_ERROR('Assertion (VARIABLE is defined) failed!\n\n' + (MESSAGE)); \
	}

/* -------------------------------------------
Group: Managing Deprecation
------------------------------------------- */

/* -------------------------------------------
Macro: DEPRECATE_SYS()
	Allow deprecation of a function that has been renamed.
	
	Replaces an old OLD_FUNCTION (which will have PREFIX_ prepended) with a NEW_FUNCTION 
	(PREFIX_ prepended) with the intention that the old function will be disabled in the future.
	
	Shows a warning in RPT each time the deprecated function is used, but runs the new function.
	
Parameters:
	OLD_FUNCTION - Full name of old function [Identifier for function that does not exist any more]
	NEW_FUNCTION - Full name of new function [Function]
	
Example:
	(begin example)
		// After renaming CBA_fnc_frog as CBA_fnc_fish
		DEPRECATE_SYS(CBA_fnc_frog,CBA_fnc_fish);
	(end)
	
Author:
	Sickboy
------------------------------------------- */
#define DEPRECATE_SYS(OLD_FUNCTION,NEW_FUNCTION) \
	OLD_FUNCTION = { \
		WARNING('Deprecated function used: OLD_FUNCTION (new: NEW_FUNCTION) in ADDON'); \
		if (isNil "_this") then { call NEW_FUNCTION } else { _this call NEW_FUNCTION }; \
	}
		
/* -------------------------------------------
Macro: DEPRECATE()
	Allow deprecation of a function, in the current component, that has been renamed.
	
	Replaces an OLD_FUNCTION (which will have PREFIX_ prepended) with a NEW_FUNCTION 
	(PREFIX_ prepended) with the intention that the old function will be disabled in the future.
	
	Shows a warning in RPT each time the deprecated function is used, but runs the new function.

Parameters:
	OLD_FUNCTION - Name of old function, assuming PREFIX [Identifier for function that does not exist any more]
	NEW_FUNCTION - Name of new function, assuming PREFIX [Function]
	
Example:
	(begin example)
		// After renaming CBA_fnc_frog as CBA_fnc_fish
		DEPRECATE(fnc_frog,fnc_fish);
	(end)
	
Author:
	Sickboy
------------------------------------------- */
#define DEPRECATE(OLD_FUNCTION,NEW_FUNCTION) \
	DEPRECATE_SYS(DOUBLES(PREFIX,OLD_FUNCTION),DOUBLES(PREFIX,NEW_FUNCTION))

/* -------------------------------------------
Macro: OBSOLETE_SYS()
	Replace a function that has become obsolete.
	
	Replace an obsolete OLD_FUNCTION with a simple COMMAND_FUNCTION, with the intention that anyone
	using the function should replace it with the simple command, since the function will be disabled in the future.
	
	Shows a warning in RPT each time the deprecated function is used, and runs the command function.

Parameters:
	OLD_FUNCTION - Full name of old function [Identifier for function that does not exist any more]
	COMMAND_CODE - Code to replace the old function [Function]
	
Example:
	(begin example)
		// In Arma2, currentWeapon command made the CBA_fMyWeapon function obsolete:
		OBSOLETE_SYS(CBA_fMyWeapon,{ currentWeapon player });
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define OBSOLETE_SYS(OLD_FUNCTION,COMMAND_CODE) \
	OLD_FUNCTION = { \
		WARNING('Obsolete function used: (use: OLD_FUNCTION) in ADDON'); \
		if (isNil "_this") then { call COMMAND_CODE } else { _this call COMMAND_CODE }; \
	}

/* -------------------------------------------
Macro: OBSOLETE()
	Replace a function, in the current component, that has become obsolete.
	
	Replace an obsolete OLD_FUNCTION (which will have PREFIX_ prepended) with a simple
	COMMAND_CODE, with the intention that anyone using the function should replace it with the simple
	command.
	
	Shows a warning in RPT each time the deprecated function is used.

Parameters:
	OLD_FUNCTION - Name of old function, assuming PREFIX [Identifier for function that does not exist any more]
	COMMAND_CODE - Code to replace the old function [Function]
	
Example:
	(begin example)
		// In Arma2, currentWeapon command made the CBA_fMyWeapon function obsolete:
		OBSOLETE(fMyWeapon,{ currentWeapon player });
	(end)
	
Author:
	Spooner
------------------------------------------- */
#define OBSOLETE(OLD_FUNCTION,COMMAND_CODE) \
	OBSOLETE_SYS(DOUBLES(PREFIX,OLD_FUNCTION),COMMAND_CODE)