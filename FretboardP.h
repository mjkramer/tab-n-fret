/* Fretboard Widget -- private definitions */


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



#ifndef FretboardP_h
#define FretboardP_h

#include  <X11/CoreP.h>
#include "Fretboard.h"



/* Fretboard class record fields */
typedef struct {
	int	ignore ;
} FretboardClassPart ;


/* Fretboard widget's full class record */
typedef struct structFretboardClassRec {
	CoreClassPart		core_class ;
	FretboardClassPart	fretboard_class ;
} FretboardClassRec ;

extern FretboardClassRec fretboardClassRec ;




/* private cursor data */
#include "lead_cursor"
#include "lead_cursor_mask"
#include "chord_cursor"
#include "chord_cursor_mask"



/* Fretboard instance fields */
typedef struct structFretboardPart {
/* resources */
	String		orientation,
			string_descriptions ;
	Cardinal	number_of_strings,
			number_of_frets ;
	Dimension	fret_width,
			nut_width,
			winding_size,
			winding_gap,
			dotmarker_percent,
			note_percent ;
	FretboardMode	mode ;
	Pixel		lead_cursor_color,
			chord_cursor_color,
			lead_cursor_background_color,
			chord_cursor_background_color,
			fretboard_color,
			fret_color,
			string_color,
			winding_color,
			winding_gap_color,
			dotmarker_color,
			nut_color,
			headstock_color,
			note_color,
			margin_text_color,
			fretboard_text_color,
			headstock_text_color ;
	XFontStruct	*font ;
	Cursor		lead_cursor,
			chord_cursor ;
/* callbacks */
	XtCallbackList	note_callback,
			mode_callback ;
/* private state */
	Dimension	width,
			length,
			top_string,
			string_spacing,
			dotmarker_size,
			note_size,
			char_width ;
	Position	x, y,
			nut_position,
			fret_spacing,
			single_dot_position,
			double_dot1_position,
			double_dot2_position,
			char_y_offset ;
	GC		fretboard_gc,
			fret_gc,
			nut_gc,
			headstock_gc,
			dotmarker_gc,
			note_gc,
			*string_gcs,
			margin_text_gc,
			fretboard_text_gc,
			headstock_text_gc ;
	Boolean		cursors_made ;
	FretboardNoteStruct	armed_note ;
	int		*chord_frets ;
	char		*string_names,
			**fret_labels ;
} FretboardPart ;


/* Fretboard widget's full instance record */
typedef struct structFretboardRec {
	CorePart	core ;
	FretboardPart	fretboard ;
} FretboardRec ;



#endif /* FretboardP_h */
