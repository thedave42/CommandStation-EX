// myAutomation.h — EXRAIL definitions for Switching Controller
//
// Copy this file to your EX-CommandStation source folder.
//
// Object definitions (PIN_TURNOUT, ROSTER) must be OUTSIDE any sequence.
// Executable commands (SET_TRACK) must be INSIDE an AUTOSTART/DONE block.

// --- Turnout definitions (visible in Engine Driver, wiThrottle, JMRI) ---
PIN_TURNOUT(1, 800, "Industry Entry")
PIN_TURNOUT(2, 801, "Industry Exit")
PIN_TURNOUT(3, 802, "Siding Entry")
PIN_TURNOUT(4, 803, "Siding Exit")
PIN_TURNOUT(5, 804, "Industry Warehouse")
PIN_TURNOUT(6, 805, "Runaround Entry")
PIN_TURNOUT(7, 806, "Runaround Exit")
PIN_TURNOUT(8, 807, "Staging 2 & 3")
PIN_TURNOUT(9, 808, "Staging 2/3 Entry")
PIN_TURNOUT(10, 809, "Yard Entry")
PIN_TURNOUT(11, 810, "Yard Exit")
PIN_TURNOUT(12, 811, "Staging 1")

// --- Routes ---
ROUTE(1, "Industry Closed")
  CLOSE(1)
  CLOSE(2)
DONE

ROUTE(2, "Industry Siding")
  THROW(1)
  THROW(2)
  THROW(3)
  THROW(4)
DONE

ROUTE(3, "Industry Runaround")
  THROW(1)
  THROW(2)
  CLOSE(3)
  CLOSE(4)
  THROW(6)
  THROW(7)
DONE

ROUTE(4, "Dry Goods Access")
  CLOSE(7)
DONE

ROUTE(5, "Warehouse Track 1")
  CLOSE(6)
  CLOSE(5)
DONE 

ROUTE(6, "Warehouse Track 2")
  CLOSE(6)
  THROW(5)
DONE 

ROUTE(7, "Yard Closed")
  CLOSE(10)
  CLOSE(11)
DONE

ROUTE(8, "Yard Siding")
  CLOSE(9)
  THROW(10)
  THROW(11)
  THROW(12)
DONE 

ROUTE(9, "Staging Track 1")
  CLOSE(9)
  CLOSE(12)
DONE 

ROUTE(10, "Staging Track 2")
  THROW(8)
  THROW(9)
DONE 

ROUTE(11, "Staging Track 3")
  CLOSE(8)
  THROW(9)
DONE 

// --- Roster ---
ROSTER(152, "EMD NW2", "Front & Rear Light/Bell/*Horn/Coupler Sounds/Dynamic Brake/*Rev Engine Up/*Rev Engine Down/Ditch Lights/Volume Mute/Startup & Shutdown/Raidiator Cooling Fan/Air Filling & Release/Brake Set & Release/Grade Crossing Horn/Passenger Annocements/Freight Announcements/Maintenance Sounds/Radio Sounds/City Background Sounds/Farm Background Sounds/Industrial Sounds/Lumber Yard Sounds/Switch to second horn/Track Sounds/Aux Light Control/*Long Horn/Play Macro/Record Macro Start & Stop/Brake Squeal")

// --- Startup sequence ---
AUTOSTART
  SET_TRACK(A, MAIN)
  SET_TRACK(B, PROG)
DONE
