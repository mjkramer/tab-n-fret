/* Fretboard class */


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


#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>
#include "FretboardP.h"
#include <malloc.h>


#define SAME 0
#define X_360_DEGREES (360*64)

typedef enum {XY, StringFret} FretCoordType ;
typedef enum {MoreToCome, FinalUndraw} UndrawNoteType ;


/* Methods */
static void	ClassInitialize(),
		Initialize(),
		Destroy(),
		Resize(),
		Redisplay(),
		arm_note(),
		make_note(),
		disarm_note(),
		finish_chord(),
		toggle_mode(),
		parse_string_descriptions(),
		make_cursor(),
		color_cursor(),
		draw_note(),
		undraw_note(),
		compute_note_position(),
		forget_chord(),
		draw_label(),
		undraw_label(),
		blank_label() ;
static Boolean 	SetValues(),
		mode_converter(),
		draw_dotmarker(),
		compute_string_fret() ;
static char	*get_label() ;



/* Translations and actions */
static char defaultTranslations[] = "\
	<Btn1Down>:	armNote()	\n\
	<Btn1Up>:	makeNote()	\n\
	<LeaveWindow>:	disarmNote()	\n\
	<Btn2Down>:	sendChord() 	\n\
	<Btn3Down>:	leadVsChord()	" ;
static XtActionsRec actionsList[] = {
	{"armNote",	(XtActionProc)arm_note},
	{"makeNote",	(XtActionProc)make_note},
	{"disarmNote",	(XtActionProc)disarm_note},
	{"sendChord", 	(XtActionProc)finish_chord},
	{"leadVsChord",	(XtActionProc)toggle_mode},
} ;



/* Default instance record values */
static XtResource resources[] = {
    {	XtNwidth,
    	XtCWidth,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, core.width), 
	XtRImmediate,
	(XtPointer)100},
    {	XtNheight,
    	XtCHeight,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, core.height), 
	XtRImmediate,
	(XtPointer)800},
    {	XtNnoteCallback,
    	XtCCallback,
	XtRCallback,
	sizeof(XtPointer),
	XtOffset(FretboardWidget, fretboard.note_callback), 
	XtRCallback,
	NULL},
    {	XtNmodeCallback,
    	XtCCallback,
	XtRCallback,
	sizeof(XtPointer),
	XtOffset(FretboardWidget, fretboard.mode_callback), 
	XtRCallback,
	NULL},
    {	XtNmode,
    	XtCMode,
	XtRFretboardMode,
	sizeof(FretboardMode),
	XtOffset(FretboardWidget, fretboard.mode), 
	XtRImmediate,
	(XtPointer)ChordMode},
    {	XtNorientation,
    	XtCOrientation,
	XtRString,
	sizeof(String),
	XtOffset(FretboardWidget, fretboard.orientation), 
	XtRString,   /* DEBUG -- should be XtROrentation */
	XtEhorizontal},
    {	XtNstrings,
    	XtCValue,
	XtRInt,
	sizeof(int),
	XtOffset(FretboardWidget, fretboard.number_of_strings), 
	XtRImmediate,
	(XtPointer)6},
    {	XtNfrets,
    	XtCValue,
	XtRInt,
	sizeof(int),
	XtOffset(FretboardWidget, fretboard.number_of_frets), 
	XtRImmediate,
	(XtPointer)22},
    {	XtNfretWidth,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.fret_width), 
	XtRImmediate,
	(XtPointer)5},
    {	XtNnutWidth,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.nut_width), 
	XtRImmediate,
	(XtPointer)15},
    {	XtNstringWidths,
    	XtCStringWidths,
	XtRString,
	sizeof(String),
	XtOffset(FretboardWidget, fretboard.string_descriptions), 
	XtRString,
	"5w 4w 3w 2p 1p 1p"},
    {	XtNwindingSize,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.winding_size), 
	XtRImmediate,
	(XtPointer)2},
    {	XtNwindingGap,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.winding_gap), 
	XtRImmediate,
	(XtPointer)1},
    {	XtNleadCursorColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.lead_cursor_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNchordCursorColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.chord_cursor_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNleadCursorBkgColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.lead_cursor_background_color), 
	XtRString,
	XtDefaultBackground},
    {	XtNchordCursorBkgColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.chord_cursor_background_color), 
	XtRString,
	XtDefaultBackground},
    {	XtNfretboardColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.fretboard_color), 
	XtRString,
	XtDefaultBackground},
    {	XtNfretColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.fret_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNstringColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.string_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNwindingColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.winding_color),
	XtRString,
	XtDefaultForeground},
    {	XtNwindingGapColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.winding_gap_color), 
	XtRString,
	XtDefaultBackground},
    {	XtNdotmarkerColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.dotmarker_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNnutColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.nut_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNheadstockColor,
    	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.headstock_color), 
	XtRString,
	XtDefaultBackground},
    {	XtNnoteColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.note_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNmarginTextColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.margin_text_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNfretboardTextColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.fretboard_text_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNheadstockTextColor,
    	XtCForeground,
	XtRPixel,
	sizeof(Pixel),
	XtOffset(FretboardWidget, fretboard.headstock_text_color), 
	XtRString,
	XtDefaultForeground},
    {	XtNdotmarkerPercent,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.dotmarker_percent), 
	XtRImmediate,
	(XtPointer)70},
    {	XtNnotePercent,
    	XtCValue,
	XtRDimension,
	sizeof(Dimension),
	XtOffset(FretboardWidget, fretboard.note_percent), 
	XtRImmediate,
	(XtPointer)85},
    {	XtNfont,
    	XtCFont,
	XtRFontStruct,
	sizeof(XFontStruct *),
	XtOffset(FretboardWidget, fretboard.font), 
	XtRString,
	XtDefaultFont},
    {	XtNleadCursor,
    	XtCCursor,
	XtRCursor,
	sizeof(Cursor),
	XtOffset(FretboardWidget, fretboard.lead_cursor), 
	XtRImmediate,
	(XtPointer)0},
    {	XtNchordCursor,
    	XtCCursor,
	XtRCursor,
	sizeof(Cursor),
	XtOffset(FretboardWidget, fretboard.chord_cursor), 
	XtRImmediate,
	(XtPointer)0},
} ;



FretboardClassRec fretboardClassRec = {
/* CoreClassPart */
    {
    (WidgetClass) &coreClassRec,	/* superclass		  */	
    "Fretboard",			/* class_name		  */
    sizeof(FretboardRec),		/* size			  */
    ClassInitialize,			/* class_initialize	  */
    NULL,				/* class_part_initialize  */
    FALSE,				/* class_inited		  */
    Initialize,				/* initialize		  */
    NULL,				/* initialize_hook	  */
    XtInheritRealize,			/* realize		  */
    actionsList,			/* actions		  */
    XtNumber(actionsList),		/* num_actions		  */
    resources,				/* resources		  */
    XtNumber(resources),		/* resource_count	  */
    NULLQUARK,				/* xrm_class		  */
    FALSE,				/* compress_motion	  */
    TRUE,				/* compress_exposure	  */
    TRUE,				/* compress_enterleave    */
    FALSE,				/* visible_interest	  */
    Destroy,				/* destroy		  */
    Resize,				/* resize		  */
    Redisplay,				/* expose		  */
    SetValues,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,		/* tm_table		  */
    XtInheritQueryGeometry,		/* query_geometry	  */
    XtInheritDisplayAccelerator,	/* display_accelerator	  */
    NULL				/* extension		  */
    },  /* CoreClass fields initialization */
    {
    0,                                     /* field not used    */
    },  /* Fretboard fields initialization */
} ;



/* public interface */
WidgetClass fretboardWidgetClass = (WidgetClass)&fretboardClassRec ;



/* Methods ================================================================= */

static void ClassInitialize(class)
WidgetClass	class ;
{
XtSetTypeConverter(XtRString,
		   XtRFretboardMode,
		   mode_converter,
		   (XtConvertArgList)NULL,
		   0,
		   XtCacheAll,
		   NULL) ;
}   /* ClassInitialize() */



static void Initialize(request, new, args, number_of_args)
FretboardWidget	request, new ;
ArgList		args ;
Cardinal	*number_of_args ;
{
FretboardPart	*past   = &request->fretboard,
		*future = &new->fretboard ;

past->cursors_made	= future->cursors_made 		= False ;
past->chord_frets	= future->chord_frets 		= NULL ;
past->fret_labels	= future->fret_labels 		= NULL ;
past->string_names	= future->string_names 		= NULL ;
past->fretboard_gc	= future->fretboard_gc		= NULL ;
past->fret_gc		= future->fret_gc		= NULL ;
past->nut_gc		= future->nut_gc		= NULL ;
past->headstock_gc	= future->headstock_gc		= NULL ;
past->dotmarker_gc	= future->dotmarker_gc		= NULL ;
past->note_gc		= future->note_gc		= NULL ;
past->string_gcs	= future->string_gcs		= NULL ;
past->margin_text_gc	= future->margin_text_gc	= NULL ;
past->fretboard_text_gc	= future->fretboard_text_gc	= NULL ;
past->headstock_text_gc	= future->headstock_text_gc	= NULL ;

SetValues(request, request, new, args, number_of_args) ;
Resize(new) ;

}   /* Initialize() */



static void Destroy(widget)
Widget widget ;
{
FretboardPart	*fretboard = &((FretboardWidget)widget)->fretboard ;
Display		*display = XtDisplay(widget) ;
int		ndx ;

/* chord frets */
free(fretboard->chord_frets) ;

/* NOTE: should XFreeColors() all fretboard.*_color here */

/* graphics contexts */
XtReleaseGC(widget, fretboard->fretboard_gc) ;
XtReleaseGC(widget, fretboard->fret_gc) ;
XtReleaseGC(widget, fretboard->nut_gc) ;
XtReleaseGC(widget, fretboard->headstock_gc) ;
XtReleaseGC(widget, fretboard->dotmarker_gc) ;
XtReleaseGC(widget, fretboard->note_gc) ;

	for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++)
	XtReleaseGC(widget, fretboard->string_gcs[ndx]) ;

/* cursors
XFreeCursor(display, fretboard->lead_cursor) ;
XFreeCursor(display, fretboard->chord_cursor) ;

/* Callbacks */
XtRemoveAllCallbacks(widget, XtNnoteCallback) ;
XtRemoveAllCallbacks(widget, XtNmodeCallback) ;

}   /* Destroy() */




static void Resize(widget)
FretboardWidget	widget ;
{
FretboardPart	*fretboard = &widget->fretboard ;
int		ndx,
		fret_spacing ;

/* horizontal:
                        | # fret  |
	|| fs | fs | nw | fs | fw | 2fw | fs ||

   vertical :
                     | #s - 1 |
	|| ss | ss/2 |   ss   | ss/2 | ss ||
*/


fret_spacing =   (  (int)widget->core.width
		  - (int)fretboard->nut_width
		  - 2 * (int)fretboard->fret_width
		  -   (int)fretboard->number_of_frets
		    * (int)fretboard->fret_width)
	       / ((int)fretboard->number_of_frets + 3) ;

/* watch out for casting here (negative result with narrow window tends
   to wrap to large positive) */
fret_spacing  =   widget->core.width
		- fretboard->nut_width
		- 2 * fretboard->fret_width
		- fretboard->number_of_frets * fretboard->fret_width ;
fret_spacing /= (int)(fretboard->number_of_frets + 3) ;

	if (fret_spacing >= 0)
	fretboard->fret_spacing = fret_spacing ;
	else
	fretboard->fret_spacing = 1 ;

fretboard->string_spacing =   widget->core.height
			    / (fretboard->number_of_strings + 2) ;

fretboard->width  = fretboard->string_spacing * fretboard->number_of_strings ;
fretboard->length =   fretboard->fret_spacing
		    + fretboard->nut_width
		    + 2 * fretboard->fret_width
		    +   fretboard->number_of_frets
		      * (fretboard->fret_spacing + fretboard->fret_width) ;

fretboard->x = ((int)widget->core.width - (int)fretboard->length) / 2 ;
fretboard->y = fretboard->string_spacing ;

fretboard->nut_position = fretboard->x + fretboard->fret_spacing ;
fretboard->top_string   = fretboard->y + fretboard->string_spacing / 2 ;


	if ((int)fretboard->string_spacing < (int)fretboard->fret_spacing) {
	fretboard->dotmarker_size = (Dimension)
				    (  (double)fretboard->string_spacing
				     * (double)fretboard->dotmarker_percent
				     / 100.0) ;
	fretboard->note_size      = (Dimension)
				    (  (double)fretboard->string_spacing
				     * (double)fretboard->note_percent
				     / 100.0) ;
	}
	else {
	fretboard->dotmarker_size = (Dimension)
				    (  (double)fretboard->fret_spacing
				     * (double)fretboard->dotmarker_percent
				     / 100.0) ;
	fretboard->note_size      = (Dimension)
				    (  (double)fretboard->fret_spacing
				     * (double)fretboard->note_percent
				     / 100.0) ;
	}

fretboard->single_dot_position  =   fretboard->y 
				  + fretboard->width / 2
				  - fretboard->dotmarker_size / 2 ;
fretboard->double_dot1_position =   fretboard->single_dot_position
				  - fretboard->string_spacing * 2 ;
fretboard->double_dot2_position =   fretboard->single_dot_position
				  + fretboard->string_spacing * 2 ;

}   /* Resize() */



static void Redisplay(widget, event, region)
FretboardWidget	widget ;
XEvent		*event ;
Region		region ;
{
FretboardPart	*fretboard = &widget->fretboard ;
Display		*display = XtDisplay(widget) ;
Window		window = XtWindow(widget) ;
int		string, fret, ndx, y ;
char		*label ;

	if (!fretboard->cursors_made) {
		if (fretboard->lead_cursor == NULL)
		make_cursor(widget,
			    XtNleadCursor,
			    &fretboard->lead_cursor,
			    fretboard->lead_cursor_color,
			    fretboard->lead_cursor_background_color) ;
		else
		color_cursor(widget,
			     fretboard->lead_cursor,
			     fretboard->lead_cursor_color,
			     fretboard->lead_cursor_background_color) ;

		if (fretboard->chord_cursor == NULL)
		make_cursor(widget,
			    XtNchordCursor,
			    &fretboard->chord_cursor,
			    fretboard->chord_cursor_color,
			    fretboard->chord_cursor_background_color) ;
		else
		color_cursor(widget,
			     fretboard->chord_cursor,
			     fretboard->chord_cursor_color,
			     fretboard->chord_cursor_background_color) ;

	fretboard->cursors_made = True ;

		if (fretboard->mode == LeadMode)
		XDefineCursor(display, window, fretboard->lead_cursor) ;
		else
		XDefineCursor(display, window, fretboard->chord_cursor) ;
	}

/* fretboard, headstock, and nut */
XFillRectangle(display,
	       window,
	       fretboard->fretboard_gc,
	       fretboard->x,
	       fretboard->y,
	       fretboard->length,
	       fretboard->width) ;
XFillRectangle(display,
	       window,
	       fretboard->headstock_gc,
	       fretboard->x,
	       fretboard->y,
	       fretboard->nut_position - fretboard->x,
	       fretboard->width) ;
XFillRectangle(display,
	       window,
	       fretboard->nut_gc,
	       fretboard->nut_position,
	       fretboard->y,
	       fretboard->nut_width,
	       fretboard->width) ;

/* frets, dot markers, and fret numbers */
	for (ndx = 0 ; ndx < fretboard->number_of_frets ; ndx++) {
	int	x ;
	char	fret_number[16] ;

	compute_note_position(fretboard, NULL, 0, ndx + 1, &x, &y) ;

	XFillRectangle(display,
		       window,
		       fretboard->fret_gc,
		       x,
		       fretboard->y,
		       fretboard->fret_width,
		       fretboard->width) ;

	draw_dotmarker(widget, display, window, ndx + 1) ;

	sprintf(fret_number, "%d", ndx + 1) ;
	draw_label(widget,
		   display,
		   window,
		   x,
		   fretboard->y - fretboard->string_spacing / 2,
		   XY,
		   fret_number,
		   fretboard->margin_text_gc) ;

	}


/* strings */
	for (ndx = 0, y = fretboard->top_string ;
	     ndx < fretboard->number_of_strings ;
	     ndx++, y += fretboard->string_spacing)
	XDrawLine(display,
		  window,
		  fretboard->string_gcs[ndx],
		  fretboard->x,
		  y,
		  fretboard->x + fretboard->length,
		  y) ;

/* chord notes */
	for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++)
		if (fretboard->chord_frets[ndx] != -1)
		draw_note(widget, NULL, ndx, fretboard->chord_frets[ndx]) ;

/* string names */
FretboardSetTuning(widget, fretboard->string_names) ;

/* note labels */
	for (string = 0 ; string < fretboard->number_of_strings ; string++)
		for (fret = 0 ; fret <= fretboard->number_of_frets ; fret++) {
		label = get_label(fretboard, NULL, string, fret) ;

			if (*label != '\0')
			draw_label(widget,
				   display,
				   window,
				   string,
				   fret,
				   StringFret,
				   label,
				   fret == 0 ? fretboard->headstock_text_gc
					     : fretboard->fretboard_text_gc) ;
		}

}   /* Redisplay() */



static Boolean SetValues(current, request, new, args, number_of_args)
Widget		current, request, new ;
ArgList		args ;
Cardinal	*number_of_args ;
{
Boolean		need_redisplay = False ;
FretboardPart	*past   = &((FretboardWidget)current)->fretboard,
		*future = &((FretboardWidget)new)->fretboard ;
XGCValues	gc_values ;
int		ndx, size ;

/* string info */
	if (   future->chord_frets	 == NULL
	    || future->number_of_strings != past->number_of_strings) {

		if (future->chord_frets != NULL)
		free(future->chord_frets) ;

	  future->chord_frets
	= (int *)malloc(future->number_of_strings * sizeof(int)) ;

		for (ndx = 0 ; ndx < future->number_of_strings ; ndx++)
		future->chord_frets[ndx] = -1 ;

	need_redisplay = TRUE ;
	}

	if (   future->fret_labels	 == NULL
	    || future->number_of_strings != past->number_of_strings
	    || future->number_of_frets	 != past->number_of_frets) {

		if (future->fret_labels != NULL) {
			for (ndx = 0 ;
			     ndx < future->number_of_strings ;
			     ndx++)
			free(future->fret_labels[ndx]) ;

		free(future->fret_labels) ;
		}

	future->fret_labels = (char **)malloc(  future->number_of_strings
	  			               * sizeof(char *)) ;

	size =   (future->number_of_frets + 1)
	       * (FRETBOARD_MAX_LABEL_LENGTH + 1) ;

		for (ndx = 0 ; ndx < future->number_of_strings ; ndx++) {
		future->fret_labels[ndx] = malloc(size) ;
		memset(future->fret_labels[ndx], '\0', size) ;
		}
	}

	if (   future->string_names	 == NULL
	    || future->number_of_strings != past->number_of_strings) {

		if (future->string_names != NULL)
		free(future->string_names) ;

	future->string_names = malloc(  future->number_of_strings
	  			      * (FRETBOARD_MAX_LABEL_LENGTH + 1)) ;
	}

/* graphics contexts (colors) */
	if (   future->fretboard_gc    == NULL
	    || future->fretboard_color != past->fretboard_color) {

		if (future->fretboard_gc != NULL)
		XtReleaseGC(new, future->fretboard_gc) ;

	gc_values.foreground = future->fretboard_color ;
	future->fretboard_gc = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->fret_gc    == NULL
	    || future->fret_color != past->fret_color) {

		if (future->fret_gc != NULL)
		XtReleaseGC(new, future->fret_gc) ;

	gc_values.foreground = future->fret_color ;
	future->fret_gc	     = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->nut_gc    == NULL
	    || future->nut_color != past->nut_color) {

		if (future->nut_gc != NULL)
		XtReleaseGC(new, future->nut_gc) ;

	gc_values.foreground = future->nut_color ;
	future->nut_gc	     = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->headstock_gc    == NULL
	    || future->headstock_color != past->headstock_color) {

		if (future->headstock_gc != NULL)
		XtReleaseGC(new, future->headstock_gc) ;

	gc_values.foreground = future->headstock_color ;
	future->headstock_gc = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->note_gc    == NULL
	    || future->note_color != past->note_color) {

		if (future->note_gc != NULL)
		XtReleaseGC(new, future->note_gc) ;

	gc_values.foreground = future->note_color ;
	future->note_gc	     = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->dotmarker_gc    == NULL
	    || future->dotmarker_color != past->dotmarker_color) {

		if (future->dotmarker_gc != NULL)
		XtReleaseGC(new, future->dotmarker_gc) ;

	gc_values.foreground = future->dotmarker_color ;
	future->dotmarker_gc = XtGetGC(new, GCForeground, &gc_values) ;
	need_redisplay	     = TRUE ;
	}

	if (   future->margin_text_gc    == NULL
	    || future->font		 != past->font
	    || future->margin_text_color != past->margin_text_color) {

		if (future->margin_text_gc != NULL)
		XtReleaseGC(new, future->margin_text_gc) ;

	gc_values.foreground   = future->margin_text_color ;
	gc_values.background   = new->core.background_pixel ;
	gc_values.font	       = future->font->fid ;
	future->margin_text_gc = XtGetGC(new,
					 GCForeground | GCBackground | GCFont,
					 &gc_values);

	future->char_width    = future->font->max_bounds.width ;
	future->char_y_offset =   future->font->ascent / 2
				- future->font->descent / 2 ;
	/* i.e. display chars vertically aligned with center of ascent,
	   with a little consideration for descenders */

	need_redisplay = TRUE ;
	}

	if (   future->fretboard_text_gc    == NULL
	    || future->font		    != past->font
	    || future->fretboard_text_color != past->fretboard_text_color) {

		if (future->fretboard_text_gc != NULL)
		XtReleaseGC(new, future->fretboard_text_gc) ;

	gc_values.foreground	  = future->fretboard_text_color ;
	gc_values.background	  = future->fretboard_color ;
	gc_values.font		  = future->font->fid ;
	future->fretboard_text_gc = XtGetGC(new,
					 GCForeground | GCBackground | GCFont,
					 &gc_values);

	future->char_width    = future->font->max_bounds.width ;
	future->char_y_offset =   future->font->ascent / 2
				- future->font->descent / 2 ;
	/* i.e. display chars vertically aligned with center of ascent,
	   with a little consideration for descenders */

	need_redisplay = TRUE ;
	}

	if (   future->headstock_text_gc    == NULL
	    || future->font		    != past->font
	    || future->headstock_text_color != past->headstock_text_color) {

		if (future->headstock_text_gc != NULL)
		XtReleaseGC(new, future->headstock_text_gc) ;

	gc_values.foreground	  = future->headstock_text_color ;
	gc_values.background	  = future->headstock_color ;
	gc_values.font		  = future->font->fid ;
	future->headstock_text_gc = XtGetGC(new,
					 GCForeground | GCBackground | GCFont,
					 &gc_values);

	future->char_width    = future->font->max_bounds.width ;
	future->char_y_offset =   future->font->ascent / 2
				- future->font->descent / 2 ;
	/* i.e. display chars vertically aligned with center of ascent,
	   with a little consideration for descenders */

	need_redisplay = TRUE ;
	}

	if (   future->string_gcs	 == NULL
	    || future->number_of_strings != past->number_of_strings
	    || future->string_color	 != past->string_color
	    || future->winding_color	 != past->winding_color
	    || future->winding_gap_color != past->winding_gap_color
	    || future->winding_size	 != past->winding_size
	    || future->winding_gap	 != past->winding_gap
	    ||    strcmp(future->string_descriptions,
			 past->string_descriptions)
	        != SAME) {

		if (future->string_gcs != NULL) {
			for (ndx = 0 ; ndx < future->number_of_strings ; ndx++)
			XtReleaseGC(new, future->string_gcs[ndx]) ;
		free(future->string_gcs) ;
		}

	future->string_gcs = (GC *)malloc(  future->number_of_strings 
					  * sizeof(GC)) ;
	parse_string_descriptions(new, future) ;

	need_redisplay = TRUE ;
	}

/* cursors */
	if (future->cursors_made) {
		if (   future->lead_cursor != past->lead_cursor
		    || future->lead_cursor_color != past->lead_cursor_color
		    ||    future->lead_cursor_background_color
		       != past->lead_cursor_background_color)
		color_cursor(new,
			     future->lead_cursor,
			     future->lead_cursor_color,
			     future->lead_cursor_background_color) ;

		if (   future->chord_cursor != past->chord_cursor
		    || future->chord_cursor_color != past->chord_cursor_color
		    ||    future->chord_cursor_background_color
		       != past->chord_cursor_background_color)
		color_cursor(new,
			     future->chord_cursor,
			     future->chord_cursor_color,
			     future->chord_cursor_background_color) ;
	}

/* correct mode cursor */
	if (future->mode != past->mode) {
	Display	*display = XtDisplay(current) ;
	Window	window   = XtWindow(current) ;

		if (future->mode == LeadMode) {
			if(    future->lead_cursor != NULL
			    && display		   != NULL
			    && window		   != NULL)
			XDefineCursor(display, window, future->lead_cursor) ;

		forget_chord(current) ;
		}

		if (   future->mode	    == ChordMode
		    && future->chord_cursor != NULL
		    && display		    != NULL
		    && window		    != NULL)
		XDefineCursor(display, window, future->chord_cursor) ;
	}

/* armed note */
future->armed_note.string = -1 ;
future->armed_note.fret   = -1 ;

return  need_redisplay ;

}   /* SetValues() */



/* Actions ================================================================= */

static void arm_note(widget, event, args, number_of_args)
FretboardWidget	widget ;
XEvent		*event ;
char		*args[] ;
int		number_of_args ;
{
XButtonEvent		*mouse = &event->xbutton ;
FretboardPart		*fretboard = &widget->fretboard ;
FretboardNoteStruct	note ;

	if (   event->type != ButtonPress && event->type    != ButtonRelease
	    || compute_string_fret(fretboard, mouse, &note) == False)
	return ;

fretboard->armed_note.string = note.string ;
fretboard->armed_note.fret   = note.fret ;

draw_note(widget, &note, 0, 0) ;

}   /* arm_note() */



static void disarm_note(widget, event, args, number_of_args)
FretboardWidget	widget ;
XEvent		*event ;
char		*args[] ;
int		number_of_args ;
{
FretboardPart	*fretboard = &widget->fretboard ;

	if (event->type != LeaveNotify)  return ;

	if (   fretboard->armed_note.string != -1
	    && fretboard->armed_note.fret   != -1)
	undraw_note(widget, &fretboard->armed_note, 0, 0, FinalUndraw) ;

fretboard->armed_note.string = -1 ;
fretboard->armed_note.fret   = -1 ;

}   /* disarm_note() */



static void make_note(widget, event, args, number_of_args)
FretboardWidget	widget ;
XEvent		*event ;
char		*args[] ;
int		number_of_args ;
{
XButtonEvent			*mouse = &event->xbutton ;
FretboardPart			*fretboard = &widget->fretboard ;
static FretboardNoteStruct	note ;

	if (event->type != ButtonPress && event->type != ButtonRelease)
	return ;

	/* only make note if a note is armed (could be button up w/o down */
	if (   fretboard->armed_note.string == -1
	    || fretboard->armed_note.fret   == -1) {
	fretboard->armed_note.string = -1 ;
	fretboard->armed_note.fret   = -1 ;
	return ;
	}

compute_string_fret(fretboard, mouse, &note) ;

	/* only make note if mouse hasn't moved since arming note */
	if (   fretboard->armed_note.string != note.string
	    || fretboard->armed_note.fret   != note.fret) {

	undraw_note(widget, &fretboard->armed_note, 0, 0, FinalUndraw) ;

	fretboard->armed_note.string = -1 ;
	fretboard->armed_note.fret   = -1 ;

	return ;
	}

	if (fretboard->mode == LeadMode) {
	static FretboardCallbackStruct	info ;

	undraw_note(widget, &fretboard->armed_note, 0, 0, FinalUndraw) ;

	fretboard->armed_note.string = -1 ;
	fretboard->armed_note.fret   = -1 ;

	info.notes	     = &note ;
	info.number_of_notes = 1 ;

	XtCallCallbacks((Widget)widget, XtNnoteCallback, (char *)&info) ;
	}
	else {   /* ChordMode */
		if (fretboard->chord_frets[note.string] != -1)
		undraw_note(widget,
			    NULL,
			    note.string,
			    fretboard->chord_frets[note.string],
			    FinalUndraw) ;

		if (fretboard->chord_frets[note.string] == note.fret)
		fretboard->chord_frets[note.string] = -1 ;
		else
		fretboard->chord_frets[note.string] = note.fret ;

	fretboard->armed_note.string = -1 ;
	fretboard->armed_note.fret   = -1 ;
	}

}   /* make_note() */



static void finish_chord(widget, event, args, number_of_args)
FretboardWidget	widget ;
XEvent		event ;
char		*args[] ;
int		number_of_args ;
{
FretboardPart			*fretboard = &widget->fretboard ;
static FretboardNoteStruct	*notes = NULL ;
static FretboardCallbackStruct	note_info ;
int				ndx, number_of_notes = 0 ;
FretboardNoteStruct		note ;

	if (notes != NULL)  free(notes) ;
notes = (FretboardNoteStruct *)malloc(  fretboard->number_of_strings
				      * sizeof(FretboardNoteStruct)) ;

	for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++)
		if (fretboard->chord_frets[ndx] != -1) {
		notes[number_of_notes].string = ndx ;
		notes[number_of_notes].fret   = fretboard->chord_frets[ndx] ;
		number_of_notes++ ;
		}

note_info.notes		  = notes ;
note_info.number_of_notes = number_of_notes ;

	if (number_of_notes > 0)
	XtCallCallbacks((Widget)widget, XtNnoteCallback, (char *)&note_info) ;

forget_chord(widget) ;
	
}   /* finish_chord() */



static void toggle_mode(widget, event, args, number_of_args)
FretboardWidget	widget ;
XEvent		event ;
char		*args[] ;
int		number_of_args ;
{
FretboardPart	*fretboard = &widget->fretboard ;
Display		*display = XtDisplay(widget) ;
Window		window   = XtWindow(widget) ;
int		ndx ;

	if (fretboard->mode == LeadMode) {
	fretboard->mode = ChordMode ;
	XDefineCursor(display, window, fretboard->chord_cursor) ;
	}
	else {
	fretboard->mode = LeadMode ;
	XDefineCursor(display, window, fretboard->lead_cursor) ;
	forget_chord(widget) ;
	}

XtCallCallbacks((Widget)widget, XtNmodeCallback, (char *)fretboard->mode) ;

}   /* toggle_mode() */



/* Converters ============================================================== */

static Boolean	mode_converter(display, args, num_args, from, to, data)
Display		*display ;
XrmValue	*args, *from, *to ;
Cardinal	*num_args ;
XtPointer	*data ;
{

	if (   strcmp((char *)(from->addr), "lead") == SAME
	    || strcmp((char *)(from->addr), "Lead") == SAME
	    || strcmp((char *)(from->addr), "LEAD") == SAME)
	*(FretboardMode *)(to->addr) = LeadMode ;
	else
	*(FretboardMode *)(to->addr) = ChordMode ;

return  True ;
}   /* mode_converter() */



/* Convenience routines ==================================================== */

void FretboardSetLabels(widget, labels)
FretboardWidget	widget ;
char		**labels ;
{
FretboardPart	*fretboard = &widget->fretboard ;
int		ndx,
		fret,
		string,
		fret_has_changes,
		*changed_strings,
		offset = 0 ;
char		*old_label, *new_label ;
UndrawNoteType	more_or_last ;

changed_strings = (int *)malloc(fretboard->number_of_strings * sizeof(int)) ;

	for (fret = 0 ; fret < fretboard->number_of_frets + 1 ; fret++) {
	fret_has_changes = FALSE ;

		for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++)
		changed_strings[ndx] = FALSE ;

		for (string = 0 ;
		     string < fretboard->number_of_strings ;
		     string++) {

		new_label =   labels[fretboard->number_of_strings - 1 - string]
			    + offset ;
		old_label = get_label(fretboard, NULL, string, fret) ;

			if (strcmp(old_label, new_label) != SAME) {
			fret_has_changes	= TRUE ;
			changed_strings[string] = TRUE ;
			strcpy(old_label, new_label) ;
			}
		}

		if (fret_has_changes) {

			for (string = 0 ;
			     string < fretboard->number_of_strings ;
			     string++)
				if (changed_strings[string])
				undraw_label(widget,
					     string,
					     fret,
					     fret == 0
					   ? fretboard->headstock_gc
					   : fretboard->fretboard_gc ) ;

		draw_dotmarker(widget, NULL, NULL, fret) ;

			for (string = 0 ;
			     string < fretboard->number_of_strings ;
			     string++)
				if (changed_strings[string]) {
					if (   fretboard->chord_frets[string]
					    == fret)
					draw_note(widget, NULL, string, fret) ;

				new_label = get_label(fretboard,
						      NULL,
						      string,
						      fret) ;
				draw_label(widget,
					   NULL,
					   NULL,
					   string,
					   fret,
					   StringFret,
					   new_label,
	   				     fret == 0
					   ? fretboard->headstock_text_gc
					   : fretboard->fretboard_text_gc) ;
				}

		}   /* if (fret_has_changes) */

	offset += FRETBOARD_MAX_LABEL_LENGTH + 1 ;
	}   /* for (fret) */

free(changed_strings) ;

}   /* FretboardSetLabels */



void FretboardSetTuning(widget, tuning)
FretboardWidget	widget ;
char		*tuning ;
{
FretboardPart	*fretboard = &widget->fretboard ;
int		string, changed = FALSE ;
char		*label ;

	if (tuning != fretboard->string_names) {
	/* because this is also called internally during redisplay */
	memcpy(fretboard->string_names,
	       tuning,
	         fretboard->number_of_strings
	       * (FRETBOARD_MAX_LABEL_LENGTH + 1)) ;
	changed = TRUE ;
	}


	for (string = fretboard->number_of_strings - 1, label = tuning ;
	     string >= 0 ;
	     string--, label += FRETBOARD_MAX_LABEL_LENGTH + 1) {

		if (changed)
		blank_label(widget,
			    string,
			    -1,
			    fretboard->margin_text_gc) ;

	draw_label(widget,
		   NULL,
		   NULL,
		   string,
		   -1,
		   StringFret,
		   label,
		   fretboard->margin_text_gc) ;
	}

}   /* FretboardSetTuning */



/* Private routines ======================================================== */

static void parse_string_descriptions(widget, fretboard)
Widget		widget ;
FretboardPart	*fretboard ;
{
char		*descriptions, *description, type ;
int		ndx, width, descriptions_malloced ;
XGCValues	gc_values ;
XtGCMask	gc_mask = 0 ;

	if (fretboard->string_descriptions == NULL) {
	descriptions = " " ;
	descriptions_malloced = FALSE ;
	}
	else {
	descriptions = malloc(strlen(fretboard->string_descriptions) + 1) ;
	strcpy(descriptions, fretboard->string_descriptions) ;
	descriptions_malloced = TRUE ;
	}

description = strtok(descriptions, " ,\t") ;

	for (ndx = fretboard->number_of_strings -1 ; ndx >= 0 ; ndx--) {
	
		if (description == NULL) {
		width = 1 ;
		type = 'p' ;
		}
		else
			switch (sscanf(description, "%d%c", &width, &type)) {
			case 0:  width = 1 ;  type = 'p' ;  break ;
			case 1:  type = 'p' ; break ;
			}

		switch (type) {
		case 'f':
		gc_values.foreground = fretboard->winding_color ;
		gc_values.line_width = width ;
		gc_mask		     = GCForeground | GCLineWidth ;
		break ;

		case 'w':
		gc_values.foreground = fretboard->winding_color ;
		gc_values.background = fretboard->winding_gap_color ;
		gc_values.line_width = width ;
		gc_values.line_style = LineDoubleDash ;
		gc_mask		     =   GCForeground
				       | GCBackground
				       | GCLineWidth
				       | GCLineStyle ;
		break ;

		case 'p':
		default:
		gc_values.foreground = fretboard->string_color ;
		gc_values.line_width = width ;
		gc_mask = GCForeground | GCLineWidth ;
		break ;
		}
	
	fretboard->string_gcs[ndx] = XtGetGC(widget, gc_mask, &gc_values) ;

		if (type == 'w') {
		char	dash_list[2] ;
		Widget	realized ;
		Display	*display ;

			/* get display from a realized parent (widget is
			   generally not realized when this is called at
			   Initialize time) */
			for (realized = widget, display = XtDisplay(realized) ;
			     display == NULL ;
			     realized = XtParent(realized)) ;

		dash_list[0] = fretboard->winding_size ;
		dash_list[1] = fretboard->winding_gap ;

		XSetDashes(display,
			   fretboard->string_gcs[ndx],
			   0,
			   dash_list,
			   2) ;
		}

		if (description != NULL)
		description = strtok(NULL, " ,\t") ;
	}


	if (descriptions_malloced)
	free(descriptions) ;

}   /* parse_string_descriptions() */



static void make_cursor(widget,
			cursor_name,
			cursor_ptr,
			foreground_color,
			background_color)
FretboardWidget	widget ;
char		*cursor_name ;
Cursor		*cursor_ptr ;
Pixel		foreground_color, background_color ;
{
FretboardPart	*fretboard = &widget->fretboard ;
Display		*display ;
Screen		*screen ;
Window		window ;
Pixmap		cursor_pixmap,
		cursor_mask_pixmap ;
XColor		cursor_foreground,
		cursor_background ;
Colormap	colormap ;
unsigned char	*cursor_bits,
		*cursor_mask_bits ;
Dimension	cursor_width,
		cursor_height,
		cursor_mask_width,
		cursor_mask_height ;
Position	cursor_x_hot,
		cursor_y_hot,
		cursor_mask_x_hot,
		cursor_mask_y_hot ;

/* get display info */
window   = XtWindow(widget) ;
display  = XtDisplay(widget) ;
screen   = XtScreen(widget) ;
colormap = DefaultColormapOfScreen(screen) ;

	if (strcmp(cursor_name, XtNleadCursor) == SAME) {
	cursor_bits   =	     lead_cursor_bits ;
	cursor_width  =	     lead_cursor_width ;
	cursor_height =	     lead_cursor_height ;
	cursor_x_hot  =	     lead_cursor_x_hot ;
	cursor_y_hot  =	     lead_cursor_y_hot ;
	cursor_mask_bits   = lead_cursor_mask_bits ;
	cursor_mask_width  = lead_cursor_mask_width ;
	cursor_mask_height = lead_cursor_mask_height ;
	cursor_mask_x_hot  = lead_cursor_mask_x_hot ;
	cursor_mask_y_hot  = lead_cursor_mask_y_hot ;
	}
	else {
	cursor_bits   =	     chord_cursor_bits ;
	cursor_width  =	     chord_cursor_width ;
	cursor_height =	     chord_cursor_height ;
	cursor_x_hot  =	     chord_cursor_x_hot ;
	cursor_y_hot  =	     chord_cursor_y_hot ;
	cursor_mask_bits   = chord_cursor_mask_bits ;
	cursor_mask_width  = chord_cursor_mask_width ;
	cursor_mask_height = chord_cursor_mask_height ;
	cursor_mask_x_hot  = chord_cursor_mask_x_hot ;
	cursor_mask_y_hot  = chord_cursor_mask_y_hot ;
	}

/* make pixmaps */
cursor_pixmap = XCreatePixmapFromBitmapData(display,
					    window,
					    (char *)cursor_bits,
					    cursor_width,
					    cursor_height,
					    1,
					    0,
					    1) ;
cursor_mask_pixmap = XCreatePixmapFromBitmapData(display,
						 window,
						 (char *)cursor_mask_bits,
						 cursor_mask_width,
						 cursor_mask_height,
						 1,
						 0,
						 1) ;


/* make cursor */
cursor_foreground.pixel  = foreground_color ;
cursor_background.pixel  = background_color ;

XQueryColor(display, colormap, &cursor_foreground) ;
XQueryColor(display, colormap, &cursor_background) ;

*cursor_ptr = XCreatePixmapCursor(display,
				  cursor_pixmap,
				  cursor_mask_pixmap,
				  &cursor_foreground,
				  &cursor_background,
				  cursor_x_hot,
				  cursor_y_hot) ;

XFreePixmap(display, cursor_pixmap) ;
XFreePixmap(display, cursor_mask_pixmap) ;

}   /* make_cursor() */



static void color_cursor(widget,
			 cursor,
			 foreground_color,
			 background_color)
Widget	widget ;
Cursor	cursor ;
Pixel	foreground_color, background_color ;
{
Display		*display ;
Screen		*screen ;
Colormap	colormap ;
XColor		cursor_foreground, cursor_background ;

display  = XtDisplay(widget) ;
screen   = XtScreen(widget) ;
colormap = DefaultColormapOfScreen(screen) ;

cursor_foreground.pixel  = foreground_color ;
cursor_background.pixel  = background_color ;

XQueryColor(display, colormap, &cursor_foreground) ;
XQueryColor(display, colormap, &cursor_background) ;

XRecolorCursor(display, cursor, &cursor_foreground, &cursor_background) ;

}   /* color_cursor() */



static Boolean compute_string_fret(fretboard, mouse, note)
FretboardPart		*fretboard ;
XButtonEvent		*mouse ;
FretboardNoteStruct	*note ;
{
int	string, fret ;

	if (   mouse->x < (int)fretboard->x
	    || mouse->x > (int)fretboard->x + (int)fretboard->length
	    || mouse->y < (int)fretboard->y
	    || mouse->y > (int)fretboard->y + (int)fretboard->width) {
	note->fret =   -1 ;
	note->string = -1 ;
	return  False ;
	}

string =   (mouse->y - (int)fretboard->y) / (int)fretboard->string_spacing ;

	if (  mouse->x
	    < (int)fretboard->nut_position + (int)fretboard->nut_width)
	fret = 0 ;
	else
	fret =     (   mouse->x
		    - (int)fretboard->nut_position
		    - (int)fretboard->nut_width)
	         / (int)(fretboard->fret_spacing + fretboard->fret_width)
	       + 1 ;

	if (fret > fretboard->number_of_frets) {
	note->fret =   -1 ;
	note->string = -1 ;
	return  False ;
	}

note->string = string ;
note->fret   = fret ;

return  True ;

}   /* compute_string_fret() */



static void draw_note(widget, note, string, fret)
FretboardWidget		widget ;
FretboardNoteStruct	*note ;   /* if NULL, use string and fret */
int			string, fret ;
{
Display		*display = XtDisplay(widget) ;
Window		window   = XtWindow(widget) ;
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y ;
char		*label ;

	if (note != NULL) {
	string = note->string ;
	fret   = note->fret ;
	}

compute_note_position(fretboard, note, string, fret, &x, &y) ;

XFillArc(display,
	 window,
	 fretboard->note_gc,
	 x - fretboard->note_size,
	 y - fretboard->note_size / 2,
	 fretboard->note_size,
	 fretboard->note_size,
	 0,
	 X_360_DEGREES) ;

	if ((label = get_label(fretboard, NULL, string, fret)) != '\0')
	draw_label(widget,
		   display,
		   window,
		   x,
		   y,
		   XY,
		   label,
		   fret == 0 ? fretboard->headstock_text_gc
			     : fretboard->fretboard_text_gc) ;

}   /* draw_note() */



static void undraw_note(widget, note, string, fret, more_or_last)
FretboardWidget		widget ;
FretboardNoteStruct	*note ;   /* if NULL, use string and fret */
int			string, fret ;
UndrawNoteType		more_or_last ;
{
Display		*display = XtDisplay(widget) ;
Window		window   = XtWindow(widget) ;
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y, begin_x, ndx ;
char		*label ;
Boolean		other_notes_here = False ;

	if (note != NULL) {
	string = note->string ;
	fret   = note->fret ;
	}

compute_note_position(fretboard, note, string, fret, &x, &y) ;

XFillArc(display,
	 window,
	 fret == 0 ? fretboard->headstock_gc : fretboard->fretboard_gc,
	 x - fretboard->note_size,
	 y - fretboard->note_size / 2,
	 fretboard->note_size,
	 fretboard->note_size,
	 0,
	 X_360_DEGREES) ;

/* possibly redraw dotmarkers, and check for notes at this fret */
	if (   more_or_last == FinalUndraw
	    && draw_dotmarker(widget, NULL, NULL, fret))

		for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++) {
			if (ndx == string)  continue ;

			if (fretboard->chord_frets[ndx] == fret) {
			other_notes_here = True ;
			break ;
			}
		}

begin_x =  x - fretboard->note_size ;
begin_x -=   (begin_x - fretboard->x)
	   % (int)(fretboard->winding_size + fretboard->winding_gap) ;

XDrawLine(display,
	  window,
	  fretboard->string_gcs[string],
	  begin_x,
	  y,
	  x,
	  y) ;

	if (other_notes_here)  /* redraw notes dotmarker messed up */
		for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++) {
			if (ndx == string)  continue ;

			if (fretboard->chord_frets[ndx] == fret)
			draw_note(widget,
				  NULL,
				  ndx,
				  fretboard->chord_frets[ndx]) ;
		}

	if ((label = get_label(fretboard, NULL, string, fret)) != '\0')
	draw_label(widget,
		   display,
		   window,
		   x,
		   y,
		   XY,
		   label,
		   fret == 0 ? fretboard->headstock_text_gc
			     : fretboard->fretboard_text_gc) ;

}   /* undraw_note() */



static void compute_note_position(fretboard, note, string, fret, x, y)
FretboardPart		*fretboard ;
FretboardNoteStruct	*note ;   /* if NULL, use string and fret */
int			string, fret, *x, *y ;
{
/* returns x and y of specified note.  y is leftmost pixel of fret,
   y is pixel of width 1 string. */

	if (note != NULL) {
	string = note->string ;
	fret   = note->fret ;
	}

	switch (fret) {
	case 0:   /* open string */
	*x = fretboard->x + fretboard->fret_spacing ;
	break ;

	case -1:   /* string tuning */
	*x = fretboard->x - fretboard->fret_spacing / 2 ;
	break ;

	default:   /* normal fret */
	*x =   fretboard->x
	     + fretboard->fret_spacing + fretboard->nut_width
	     + (fretboard->fret_spacing + fretboard->fret_width) * fret
	     - fretboard->fret_width ;
	}

*y = fretboard->top_string + fretboard->string_spacing * string ;

}   /* compute_note_position() */



static Boolean draw_dotmarker(widget, display, window, fret)
FretboardWidget	widget ;
Display		*display ;	/* if NULL, will compute from widget */
Window		window ;	/* if NULL, will compute from widget */
int		fret ;
{
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y ;
static int	dotted_frets[] = {0,0,0,1,0,1,0,1,0,1,0,0,2,
				    0,0,1,0,1,0,1,0,1,0,0,2} ;

	if (fret > sizeof(dotted_frets) / sizeof(int))  return  False ;

	if (dotted_frets[fret] == 0)  return  False ;

	if (window  == NULL)  window  = XtWindow(widget) ;
	if (display == NULL)  display = XtDisplay(widget) ;

compute_note_position(fretboard, NULL, 0, fret, &x, &y) ;
x -= fretboard->fret_spacing   / 2 ;
x -= fretboard->dotmarker_size / 2 ;

	switch (dotted_frets[fret]) {
	case 1:
	XFillArc(display,
		 window,
		 fretboard->dotmarker_gc,
		 x,
		 fretboard->single_dot_position,
		 fretboard->dotmarker_size,
		 fretboard->dotmarker_size,
		 0,
		 X_360_DEGREES) ;
	break ;

	case 2:
	XFillArc(display,
		 window,
		 fretboard->dotmarker_gc,
		 x,
		 fretboard->double_dot1_position,
		 fretboard->dotmarker_size,
		 fretboard->dotmarker_size,
		 0,
		 X_360_DEGREES) ;
	XFillArc(display,
		 window,
		 fretboard->dotmarker_gc,
		 x,
		 fretboard->double_dot2_position,
		 fretboard->dotmarker_size,
		 fretboard->dotmarker_size,
		 0,
		 X_360_DEGREES) ;
	break ;
	}

return  True ;

}   /* draw_dotmarker() */



static void forget_chord(widget)
FretboardWidget	widget ;
{
FretboardPart		*fretboard = &widget->fretboard ;
int			ndx, ndx2 ;

	/* check all chord notes */
	for (ndx = 0 ; ndx < fretboard->number_of_strings ; ndx++)

		if (fretboard->chord_frets[ndx] != -1) {
		/* found one to delete */

			/* but delete all others at this fret first */
			for (ndx2 = ndx + 1 ;
			     ndx2 < fretboard->number_of_strings ;
			     ndx2++)

				if (   fretboard->chord_frets[ndx2] 
				    == fretboard->chord_frets[ndx]) {
				undraw_note(widget,
					    NULL,
					    ndx2,
					    fretboard->chord_frets[ndx2],
					    MoreToCome) ;
				fretboard->chord_frets[ndx2] = -1 ;
				}

		/* and then delete first, telling undraw_note it is last */
		undraw_note(widget,
			    NULL,
			    ndx,
			    fretboard->chord_frets[ndx],
			    FinalUndraw) ;
		fretboard->chord_frets[ndx] = -1 ;
		}

}   /* forget_chord() */



static void draw_label(widget,
		       display,
		       window,
		       x_or_string,
		       y_or_fret,
		       coord_type,
		       string,
		       graphics_context)
FretboardWidget	widget ;
Display		*display ;	/* if NULL, will compute from widget */
Window		window ;	/* if NULL, will compute from widget */
int		x_or_string, y_or_fret ;
FretCoordType	coord_type ;
char		*string ;
GC		graphics_context ;
{
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y, length = strlen(string) ;

	if (window  == NULL)  window  = XtWindow(widget) ;
	if (display == NULL)  display = XtDisplay(widget) ;

	if (coord_type == XY) {
	x = x_or_string ;
	y = y_or_fret ;
	}
	else
	compute_note_position(fretboard, NULL, x_or_string, y_or_fret, &x, &y);

x -= fretboard->char_width * length ;
y += fretboard->char_y_offset ;

XDrawString(display, window, graphics_context, x, y, string, length) ;

}   /* draw_label() */



static void undraw_label(widget, string, fret, graphics_context)
FretboardWidget	widget ;
int		string, fret ;
GC		graphics_context ;
{
Display		*display  = XtDisplay(widget) ;
Window		window  = XtWindow(widget) ;
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y, begin_x ;

compute_note_position(fretboard, NULL, string, fret, &x, &y);

XFillRectangle(display,
	       window,
	       graphics_context,
	       x - fretboard->fret_spacing,
	       y - fretboard->string_spacing / 2,
	       fretboard->fret_spacing,
	       fretboard->string_spacing) ;

begin_x =  x - fretboard->fret_spacing ;
begin_x -=   (x - fretboard->x)
	   % (int)(fretboard->winding_size + fretboard->winding_gap) ;

XDrawLine(display,
	  window,
	  fretboard->string_gcs[string],
	  begin_x,
	  y,
	  x,
	  y) ;

}   /* undraw_label() */



static void blank_label(widget, string, fret, graphics_context)
FretboardWidget	widget ;
int		string, fret ;
GC		graphics_context ;
{
Display		*display  = XtDisplay(widget) ;
Window		window  = XtWindow(widget) ;
FretboardPart	*fretboard = &widget->fretboard ;
int		x, y ;
static char	*blanks = "                                               " ;

compute_note_position(fretboard, NULL, string, fret, &x, &y);

x -= fretboard->char_width * FRETBOARD_MAX_LABEL_LENGTH ;
y += fretboard->char_y_offset ;

XDrawImageString(display,
		 window,
		 graphics_context,
		 x,
		 y,
		 blanks,
		 FRETBOARD_MAX_LABEL_LENGTH) ;

}   /* blank_label() */



static char *get_label(fretboard, note, string, fret)
FretboardPart		*fretboard ;
FretboardNoteStruct	*note ;   /* if NULL, use string and fret */
int			string, fret ;
{

	if (note != NULL) {
	string = note->string ;
	fret   = note->fret ;
	}

return    fretboard->fret_labels[string]
	+ fret * (FRETBOARD_MAX_LABEL_LENGTH + 1) ;

}   /* get_label() */
