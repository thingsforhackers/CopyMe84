/*
 * tune.c
 *
 *  Created on: 11 Jun 2016
 *      Author: njohn
 */

#include "notes.h"

#include <avr/pgmspace.h> // needed for PROGMEM

const uint16_t noteArray2[] PROGMEM = {
    C4, E4, G4, C5, E5, G4, C5, E5
};

const uint16_t noteArray[] PROGMEM = {
    C4, E4, G4, C5, E5, G4, C5, E5, //measures 1-2, notes 1-32
    C4, E4, G4, C5, E5, G4, C5, E5,
    C4, D4, A4, D5, F5, A4, D5, F5,
    C4, D4, A4, D5, F5, A4, D5, F5,
    B3, D4, G4, D5, F5, G4, D5, F5, //measures 3-4, notes 33-64
    B3, D4, G4, D5, F5, G4, D5, F5,
    C4, E4, G4, C5, E5, G4, C5, E5,
    C4, E4, G4, C5, E5, G4, C5, E5,
    C4, E4, A4, E5, A5, A4, E5, A5, //measures 5-6, notes 65-96
    C4, E4, A4, E5, A5, A4, E5, A5,
    C4, D4, FS4, A4, D5, FS4, A4, D5,
    C4, D4, FS4, A4, D5, FS4, A4, D5,
    B3, D4, G4, D5, G5, G4, D5, G5, //measures 7-8, notes 97-128
    B3, D4, G4, D5, G5, G4, D5, G5,
    B3, C4, E4, G4, C5, E4, G4, C5,
    B3, C4, E4, G4, C5, E4, G4, C5,
    A3, C4, E4, G4, C5, E4, G4, C5, //measures 9-10, notes 129-160
    A3, C4, E4, G4, C5, E4, G4, C5,
    D3, A3, D4, FS4, C5, D4, FS4, C5,
    D3, A3, D4, FS4, C5, D4, FS4, C5,
    G3, B3, D4, G4, B4, D4, G4, B4, //measures 11-12, notes 161-192
    G3, B3, D4, G4, B4, D4, G4, B4,
    G3, AS3, E4, G4, CS5, E4, G4, CS5,
    G3, AS3, E4, G4, CS5, E4, G4, CS5,
    F3, A3, D4, A4, D5, D4, A4, D5, //measures 13-14, notes 193-224
    F3, A3, D4, A4, D5, D4, A4, D5,
    F3, GS3, D4, F4, B4, D4, F4, B4,
    F3, GS3, D4, F4, B4, D4, F4, B4,
    E3, G3, C4, G4, C5, C4, G4, C5, //measures 15-16, notes 225-256
    E3, G3, C4, G4, C5, C4, G4, C5,
    E3, F3, A3, C4, F4, A3, C4, F4,
    E3, F3, A3, C4, F4, A3, C4, F4,
    D3, F3, A3, C4, F4, A3, C4, F4, //measures 17-18, notes 257-288
    D3, F3, A3, C4, F4, A3, C4, F4,
    G2, D3, G3, B3, F4, G3, B3, F4,
    G2, D3, G3, B3, F4, G3, B3, F4,
    C3, E3, G3, C4, E4, G3, C4, E4, //measures 19-20, notes 289-320
    C3, E3, G3, C4, E4, G3, C4, E4,
    C3, G3, AS3, C4, E4, AS3, C4, E4,
    C3, G3, AS3, C4, E4, AS3, C4, E4,
    F2, F3, A3, C4, E4, A3, C4, E4, //measures 21-22, notes 321-352
    F2, F3, A3, C4, E4, A3, C4, E4,
    FS2, C3, A3, C4, DS4, A3, C4, DS4,
    FS2, C3, A3, C4, DS4, A3, C4, DS4,
    GS2, F3, B3, C4, D4, B3, C4, D4, //measures 23-24, notes 353-384
    GS2, F3, B3, C4, D4, B3, C4, D4,
    G2, F3, G3, B3, D4, G3, B3, D4,
    G2, F3, G3, B3, D4, G3, B3, D4,
    G2, E3, G3, C4, E4, G3, C4, E4, //meaures 25-26, notes 385-416
    G2, E3, G3, C4, E4, G3, C4, E4,
    G2, D3, G3, C4, F4, G3, C4, F4,
    G2, D3, G3, C4, F4, G3, C4, F4,
    G2, D3, G3, B3, F4, G3, B3, F4, //measures 27-28, notes 417-448
    G2, D3, G3, B3, F4, G3, B3, F4,
    G2, DS3, A3, C4, FS4, A3, C4, FS4,
    G2, DS3, A3, C4, FS4, A3, C4, FS4,
    G2, E3, G3, C4, G4, G3, C4, G4, //measures 29-30, notes 449-480
    G2, E3, G3, C4, G4, G3, C4, G4,
    G2, F3, G3, C4, F4, G3, C4, F4,
    G2, F3, G3, C4, F4, G3, C4, F4,
    G2, F3, G3, B3, F4, G3, B3, F4, //measures 31-32, notes 481-512
    G2, F3, G3, B3, F4, G3, B3, F4,
    C2, C3, G3, AS3, E4, G3, AS3, E4,
    C2, C3, G3, AS3, E4, G3, AS3, E4,
    C2, C3, F3, A3, C4, F4, C4, A3, //measures 33-34, notes 513-544
    C4, A3, F3, A3, F3, D3, F3, D3,
    C2, B2, G4, B4, D5, F5, D5, B4,
    D5, B4, G4, B4, D4, F4, E4, D4, };
