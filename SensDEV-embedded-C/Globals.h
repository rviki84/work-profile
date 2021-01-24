//***************************************************************
//@doc
// Modulname:    $Source: $
// User:         $Author:  $
// Version:      $Name: $  $Revision: $
// Datum:        $Date:  $
// Qualität:     $State:  $
//***************************************************************
//@module <filename> | Beschreibung:
//***************************************************************
//@comm Historie:<nl>
//==========<nl>
// 06.09.2001<nl>
// Anpassung an Autoduck<nl>
//***************************************************************

#ifndef GLOBALS_H
  #define GLOBALS_H  //@define H-File Bezeichner

//***************************************************************
// Definitionen ueber C-Compiler
//***************************************************************

#ifdef __C51__
  #if __C51__ < 610
    #error Fehler_0001:                          
    #error Fehler: Diese Beispiele sind mit dem C-Compiler Ver > 6.10 getestet worden.
  #endif
#endif 

#include <def_mic.h>
//***************************************************************
// Angaben ueber den verwendeten Mikrocontroller und dessen Umfeld
//***************************************************************
#define MIKROPRO             _80517   //@define welcher Prozessor wird verwendet
#define SYSTEM_CLKHZ      12000000L   //@define Angabe in Hz
#define MACHINE_CYCLE             1   //@define Angabe in µSec 12 MHz Taktfrequenz
#define SYSTEM_CLK   SYSTEM_CLKHZ/1000000L  //@define Berechnung in MHz


//***************************************************************
// Auswertung der Angaben
//***************************************************************
// Auswertung des verwendeten Mikrocontrollers
    #if MIKROPRO == _8031 
      #include <reg51.h>
    #elif MIKROPRO == _8032
      #include <reg52.h>
    #elif MIKROPRO == _80515
      #include <reg515.h>
    #elif MIKROPRO == _80517
      #include <reg517.h> 
    #elif MIKROPRO == _80517A
      #include <reg517a.h>
    #elif MIKROPRO == _501
      #include <reg501.h>
    #elif MIKROPRO == _517
      #include <reg517.h>
    #elif MIKROPRO == _517A
      #include <reg517a.h>
    #else
      #error Fehler_0002:                          
      #error Fehler: Kein Mikroprozessor in GLOBALS.H definiert 
    #endif // Endekennung für MICROPRO-Abfrage

#include <def_int.h>
#include <def_bits.h>    // !! Neu !! zusaetzlicher Eintrag
//***************************************************************
// Angaben über die verwendete Peripherie 
//***************************************************************

//***************************************************************
#endif // Endekennung für GLOBALS-Abfrage

