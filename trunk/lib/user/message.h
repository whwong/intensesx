/**
 * @file message.h
 * @brief Message codes
 * @author Daniel Szot
 */

#ifndef _USER_MESSAGE_H
#define _USER_MESSAGE_H

struct msg
{
        // For future use - windows to which the message is directed
    	struct guiWindow *wnd;
	UINT32 message;
	UINT32 param1;
	UINT32 param2;
	UINT32 timestamp;
	UINT16 ptX;
	UINT16 ptY;
};

#define MSG_THREAD  (0x0001 << 16)
#define MSG_WINDOW  (0x0002 << 16)
#define MSG_INPUT   (0x0004 << 16)

#define MSG_QUIT    (MSG_THREAD | 0x0012)

// Pointer Messages
#define MSG_POINTERFIRST    (MSG_INPUT | 0x0001)
#define MSG_POINTERMOVE     (MSG_INPUT | 0x0001)
#define MSG_POINTERDOWN     (MSG_INPUT | 0x0002)
#define MSG_POINTERUP       (MSG_INPUT | 0x0003)
#define MSG_POINTERDBCLICK  (MSG_INPUT | 0x0004)
#define MSG_POINTERLEAVE    (MSG_INPUT | 0x0005)
#define MSG_POINTERHOVER    (MSG_INPUT | 0x0006)
#define MSG_POINTERLAST     (MSG_INPUT | 0x0006)

// Get X and Y coordinates from message param2
// Vlues are represented as UINT16
#define GET_X_PARAM2(p) (p & 0xFFFF)
#define GET_Y_PARAM2(p) ((p >> 16) & 0xffff)

// Get delta X and Y values from param1
// For example to get speed values from MSG_POINTERMOVE
// Values are represented as INT8
#define GET_DX_PARAM1(p) ((INT8)((p >> 16) & 0xFF))
#define GET_DY_PARAM1(p) ((INT8)((p >> 24) & 0xFF))

// Key Messages
#define MSG_KEYFIRST        (MSG_INPUT | 0x0011)
#define MSG_KEYDOWN         (MSG_INPUT | 0x0011)
#define MSG_KEYUP           (MSG_INPUT | 0x0012)
#define MSG_CHAR            (MSG_INPUT | 0x0013)
#define MSG_KEYLAST         (MSG_INPUT | 0x0013)

// Disk Messages
#define MSG_DISKFIRST       (MSG_INPUT | 0x0014)
#define MSG_DISKINSERTED    (MSG_INPUT | 0x0014)
#define MSG_DISKREMOVED     (MSG_INPUT | 0x0015)
#define MSG_DISKLAST        (MSG_INPUT | 0x0015)

// Window messages
#define MSG_WINDOWFIRST     (MSG_WINDOW | 0x0001)
#define MSG_CREATE          (MSG_WINDOW | 0x0001)
#define MSG_NCCREATE        (MSG_WINDOW | 0x0002)
#define MSG_PAINT           (MSG_WINDOW | 0x0003)
#define MSG_NCPAINT         (MSG_WINDOW | 0x0004)
#define MSG_ENABLE          (MSG_WINDOW | 0x0005)
#define MSG_SETTEXT         (MSG_WINDOW | 0x0006)
#define MSG_GETTEXT         (MSG_WINDOW | 0x0007)
#define MSG_SETFONT         (MSG_WINDOW | 0x0008)
#define MSG_COMMAND         (MSG_WINDOW | 0x0009)
#define MSG_WINDOWLAST      (MSG_WINDOW | 0x0009)

#endif
