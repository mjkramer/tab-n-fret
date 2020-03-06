/* Fretboard Widget -- public definitions */


/*
This code is released into the public domain without any express or
implied warranty.  The author accepts no responsibility for any
consequences arising from its use.

This code may be distributed and modified in any way; however, please
retain notice of original authorship in this and any derivative work.

Version:	1.00
Date:		9/20/93

revision history:
1.00	 9/20/93	First release.  Posted to alt.sources
*/



#ifndef Fretboard_h
#define Fretboard_h



/* Resources:
Fretboard-specific resources:

 Name			Class		RepType		Default Value
 ----			-----		-------		-------------
 orientation		Orientation	String		XtEhorizontal
 strings		Value		Cardinal	6
 frets			Value		Cardinal	22
 fretWidth		Value		Dimension	10
 nutWidth		Value		Dimension	20
 stringWidths		StringWidths	String		"5w 4w 3w 2p 1p 1p"
 windingSize		Dimension	Dimension	2
 windingGap		Dimension	Dimension	1
 dotmarkerPercent	Dimension	Dimension	70
 notePercent		Dimension	Dimension	85
 leadCursor		Cursor		Cursor		compiled-in shape
 chordCursor		Cursor		Cursor		compiled-in shape
 leadCursorColor	Foreground	Pixel		XtDefaultForeground
 chordCursorColor	Foreground	Pixel		XtDefaultForeground
 leadCursorBkgColor	Foreground	Pixel		XtDefaultBackground
 chordCursorBkgColor	Foreground	Pixel		XtDefaultBackground
 fretboardColor		Background	Pixel		XtDefaultBackground
 fretColor		Foreground	Pixel		XtDefaultForeground
 stringColor		Foreground	Pixel		XtDefaultForeground
 windingColor		Foreground	Pixel		XtDefaultForeground
 windingGapColor	Background	Pixel		XtDefaultBackground
 dotmarkerColor		Foreground	Pixel		XtDefaultForeground
 nutColor		Foreground	Pixel		XtDefaultForeground
 headstockColor		Background	Pixel		XtDefaultBackground
 noteColor		Foreground	Pixel		XtDefaultForeground
 marginTextColor	Foreground	Pixel		XtDefaultForeground
 fretboardTextColor	Foreground	Pixel		XtDefaultForeground
 headstockTextColor	Foreground	Pixel		XtDefaultForeground
 mode			Mode		FretboardMode	ChordMode
 font			Font		XFontStruct*	XtDefaultFont
 noteCallback		Callback	Pointer		NULL
 modeCallback		Callback	Pointer		NULL

Resources inherited from core:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 accelerators	     Accelerators	AccelerationTable NULL	     
 ancestorSensitive   AncestorSensitive	Boolean		True
 background	     Background		Pixel		XtDefaultBackground
 backgroundPixmap    Pixmap		Pixel		XtUnspecifiedPixmap
 borderColor	     BorderColor	Pixel		XtDefaultForeground
 borderPixmap	     Pixmap		Pixel		XtUnspecifiedPixmap
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 height		     Height		Dimension	100 (changed from 0)
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 screen		     Screen		Screen		parent's screen
 sensitive	     Sensitive		Boolean		True
 translations	     Translations	TranslationTable NULL
 width		     Width		Dimension	800 (changed from 0)
 x		     Position		Position	0
 y		     Position		Position	0

*/



/* Resource names */
#define XtNstrings	       "strings"
#define XtNfrets	       "frets"
#define XtNfretWidth	       "fretWidth"
#define XtNnutWidth	       "nutWidth"
#define XtNstringWidths	       "stringWidths"
#define XtNwindingSize	       "windingSize"
#define XtNwindingGap	       "windingGap"
#define XtNdotmarkerPercent    "dotmarkerPercent"
#define XtNnotePercent	       "notePercent"
#define XtNleadCursor	       "leadCursor"
#define XtNchordCursor	       "chordCursor"
#define XtNleadCursorColor     "leadCursorColor"
#define XtNchordCursorColor    "chordCursorColor"
#define XtNleadCursorBkgColor  "leadCursorBkgColor"
#define XtNchordCursorBkgColor "chordCursorBkgColor"
#define XtNfretboardColor      "fretboardColor"
#define XtNfretColor	       "fretColor"
#define XtNstringColor	       "stringColor"
#define XtNwindingColor	       "windingColor"
#define XtNwindingGapColor     "windingGapColor"
#define XtNdotmarkerColor      "dotmarkerColor"
#define XtNnutColor	       "nutColor"
#define XtNheadstockColor      "headstockColor"
#define XtNnoteColor	       "noteColor"
#define XtNmarginTextColor     "marginTextColor"
#define XtNfretboardTextColor  "fretboardTextColor"
#define XtNheadstockTextColor  "headstockTextColor"
#define XtNmode		       "mode"
#define XtNnoteCallback	       "noteCallback"
#define XtNmodeCallback	       "modeCallback"

/* Resource classes */
#define XtCMode		       "Mode"
#define XtCStringWidths	       "StringWidths"



/* Data types */
#ifndef FretboardMode_datatypes
#define FretboardMode_datatypes

typedef struct {
	int	string, fret ;
} FretboardNoteStruct ;

typedef struct {
	int			number_of_notes ;
	FretboardNoteStruct	*notes ;
} FretboardCallbackStruct ;


#define XtRFretboardMode "FretboardMode"
typedef enum {ChordMode, LeadMode, UnknownMode}  FretboardMode ;


#define FRETBOARD_MAX_LABEL_LENGTH 3

#endif   /* ifndef FretboardMode_datatypes */



/* Callback info */
/* noteCallback:
	returns malloc'd FretboardCallbackStruct* with
		malloc'd FretboardNoteStruct's
	widget will free all at next noteCallback		*/
/* modeCallback:
	returns  FretboardMode					*/



/* Convenience functions */
extern void	FretboardSetLabels(
#ifdef NEED_PROTOTYPES
	FretboardWidget	widget ;
	char		**labels ;
	/* number_of_strings pointers to labels, each one pointing to:
	   ((number_of_frets + 1) * (FRETBOARD_MAX_LABEL_LENGTH + 1))
	   individually null-terminated label strings, e.g.:
	   "b3 \03  \0 ..."  Pointers are for strings low-to-high */
#endif
) ;

extern void	FretboardSetTuning(
#ifdef NEED_PROTOTYPES
	FretboardWidget	widget ;
	char		*tuning ;
	/* string of length:
	   number_of_strings * (FRETBOARD_MAX_LABEL_LENGTH + 1)
	   with individually null-terminated label strings, e.g.:
	   "E  \0A  \0...", ordered low-to-high */
#endif
) ;


/* Class and instance definitions */
extern WidgetClass     fretboardWidgetClass;

typedef struct structFretboardClassRec	*FretboardWidgetClass;
typedef struct structFretboardRec	*FretboardWidget;


#endif /* Fretboard_h */
