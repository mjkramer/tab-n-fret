#include <malloc.h>
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include "Fretboard.h"
#include "XFretboard.h"



static char	*program_name ;
static void	gui_init(),
		gui_run() ;




/* -------------------------------  main()  ------------------------------- */
main(argc, argv)
int	argc ;
char 	**argv ;
{
program_name = argv[0] ;

gui_init(argc, argv) ;
gui_run() ;
}   /* main() */



#define WC(CLASS, WIDGET, NAME, PARENT) \
	WIDGET = XtCreateManagedWidget(NAME, CLASS, PARENT, NULL, 0)

#define CB(WIDGET, CALLBACK, FUNCTION, CLIENT_DATA) \
	 XtAddCallback(WIDGET, CALLBACK, FUNCTION, (XtPointer)CLIENT_DATA)

#define PC(CLASS, WIDGET, NAME, PARENT) \
	WIDGET = XtCreatePopupShell(NAME, CLASS, PARENT, NULL, 0)


#define NUMBER_OF_STRINGS 6
#define MAX_NOTES	  12
#define SAME		  0
#define NAMES_NORMAL	  1
#define NAMES_12TONE	  2
#define TOKEN_SEPARATOR	  " ,\t"
#define NO_NOTE		  "-"


typedef struct {
	char	notes[MAX_NOTES][FRETBOARD_MAX_LABEL_LENGTH + 1] ;
} ChordScaleDescript ;


#define BOX boxWidgetClass
#define BUT commandWidgetClass
#define FRM formWidgetClass
#define FRT fretboardWidgetClass
#define LBL labelWidgetClass
#define PAN panedWidgetClass
#define TOG toggleWidgetClass
#define TRN transientShellWidgetClass
#define TXT asciiTextWidgetClass

static void	make_widgets(),
		label_fretboard(),
		label_degree_names(),
		make_chord_scales(),
		exitCB(),
		noteCB(),
		commandCB(),
		internalModeCB(),
		externalModeCB(),
		keyCB(),
		chordScaleCB(),
		spellingCB(),
		intervalModeCB(),
		tuningCB(),
		popupCB(),
		popdownCB() ;
static int	name_to_number(),
		count_tokens() ;
static char	*number_to_name(),
		*safe_strtok() ;
		


static FretboardMode	global_mode = UnknownMode ;
static char		*string_fret_labels[NUMBER_OF_STRINGS],
			*degree_names[MAX_NOTES],
			tuning_text[  NUMBER_OF_STRINGS
				    * (FRETBOARD_MAX_LABEL_LENGTH + 1)] ;
static Cardinal		number_of_frets ;
static int		numeric_tuning[NUMBER_OF_STRINGS],
			relative_chord_scale[MAX_NOTES],
			absolute_chord_scale[MAX_NOTES],
			number_of_chords,
			number_of_scales ;
static XtAppContext	application_context ;
static FILE		*output ;

static ChordScaleDescript	*chord_scales ;
static ChordScaleDescript	chromatic_scale = {"rt",  "b2",   "2",  "b3",
						   "3",   "4",   "b5",   "5",
						  "b6",   "6",   "b7",   "7"} ;


XrmOptionDescRec CommandlineOptions[] = {
{"-emacs",	".emacs",	XrmoptionNoArg,	(caddr_t)"False"},
{"+emacs",	".emacs",	XrmoptionNoArg,	(caddr_t)"True"},
} ;


static struct	application_resources {
	Boolean	emacs ;
	char	*chords,
		*scales ;
} application_resources ;

static struct	spelling_resources {
	char	*spelling ;
} spelling_resources ;


static XtResource	applicationResources[] = {
    {	"emacs",
	"Emacs",
	XtRBoolean,
	sizeof(Boolean),
	XtOffset(struct application_resources *, emacs),
	XtRImmediate,
	(XtPointer)True},
    {	"chordList",
	"ChordList",
	XtRString,
	sizeof(String),
	XtOffset(struct application_resources *, chords),
	XtRString,
	""},
    {	"scaleList",
	"ScaleList",
	XtRString,
	sizeof(String),
	XtOffset(struct application_resources *, scales),
	XtRString,
	""},
} ;
static XtResource	spellingResources[] = {
    {	"spelling",
	"Spelling",
	XtRString,
	sizeof(String),
	XtOffset(struct spelling_resources *, spelling),
	XtRString,
	""},
} ;



static Widget	ApplicationShell,	/* level 0 */
		Frame,			/* level 1 */
		HelpPopup,
		Fretboard,		/* level 2 -- children of Frame */
		Emacs1,
		Emacs2,
		Keys,
		Chords,
		Scales,
		Spelling,
		Tunings,
		HelpForm,		/* level 2 -- children of HelpPopup */
		Emacs1Title,		/* level 3 -- children of Emacs1 */
		Chord,
		Lead,
		NewStaff,
		Barline,
		HammerOn,
		PullOff,
		Bend,
		Release,
		SlideUp,
		SlideDown,
		GhostNote,
		MuffledNote,
		NormalNote,
		Emacs2Title,		/* level 3 -- children of Emacs2 */
		BackBar,
		BackNote,
		FwrdNote,
		FwrdBar,
		UpStaff,
		UpNote,
		DownNote,
		DownStaff,
		PreviousNote,
		DeleteNote,
		SqueezeChord,
		InsertSpace,
		KeyTitle,		/* level 3 -- children of Keys */
		EKey,
		FKey,
		FsharpKey,
		GKey,
		AflatKey,
		AKey,
		BflatKey,
		BKey,
		CKey,
		CsharpKey,
		DKey,
		EflatKey,
		ChordTitle,		/* level 3 -- children of Chords */
		ChordRoot,
		Chord5,
		ChordMajor,
		ChordMinor,
		ChordSus2,
		ChordSus4,
		Chord7,
		ChordMinor7,
		ChordMajor7,
		Chord6,
		ChordMinor6,
		Chord9,
		ChordMinor9,
		ChordDiminished,
		Chord11,
		ChordMinor11,
		Chord13,
		ChordMinor13,
		ScaleTitle,		/* level 3 -- children of Scales */
		NoScale,
		ChromaticScale,
		MajorScale,
		MinorScale,
		BluesScale,
		MajorPentatonicScale,
		MinorPentatonicScale,
		DorianMode,
		PhrygianMode,
		LydianMode,
		MixolydianMode,
		AeolianMode,
		LocrianMode,
		SpellingTitle,		/* level 3 -- children of Spelling */
		IntervalWidgets[MAX_NOTES],
		Interval0,
		Interval1,
		Interval2,
		Interval3,
		Interval4,
		Interval5,
		Interval6,
		Interval7,
		Interval8,
		Interval9,
		Interval10,
		Interval11,
		TuningTitle,		/* level 3 -- children of Tunings */
		TuningText,
		TuningTextSrc,
		ModeNormal,
		Mode12Tone,
		Help,
		Quit,
		HelpText,		/* level 3 -- children of HelpForm */
		HelpOkay ;



static void gui_init(argc, argv)   /* -------------------------------------- */
int	argc ;
char 	**argv ;
{
int		string, ndx ;
Cardinal	cargc = argc ;

ApplicationShell = XtAppInitialize(&application_context,
				   "XFretboard",
				   CommandlineOptions,
				   XtNumber(CommandlineOptions),
				   &cargc,
				   argv,
				   fallback_resources,
				   NULL,
				   0) ;

	if (cargc > 1) {
	fprintf(stderr, "Usage: %s [-emacs] [+emacs]\n", program_name) ;
	fprintf(stderr,
		"\t-emacs\tturn off emacs output (for standalone use)\n") ;
	fprintf(stderr, "\t+emacs\tturn on emacs output)\n") ;
	exit(1) ;
	}

XtGetApplicationResources(ApplicationShell,
			  (XtPointer)&application_resources,
			  applicationResources,
			  XtNumber(applicationResources),
			  NULL,
			  0) ;

	if (application_resources.emacs)
	output = stdout ;
	else if ((output = fopen("/dev/null", "w")) == NULL)
	output = stdout ;

make_widgets(application_context) ;

XtVaGetValues(Fretboard, XtNfrets, &number_of_frets, NULL, NULL) ;

	for (string = 0 ; string < NUMBER_OF_STRINGS ; string++)
	  string_fret_labels[string]
	= malloc((number_of_frets + 1) * (FRETBOARD_MAX_LABEL_LENGTH + 1)) ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
	degree_names[ndx] = malloc(FRETBOARD_MAX_LABEL_LENGTH + 1) ;
label_degree_names(0) ;

}   /* gui_init() */



static void gui_run() {   /* ----------------------------------------------- */
XtRealizeWidget(ApplicationShell) ;
tuningCB(TuningText, NULL, NULL) ;
XtAppMainLoop(application_context) ;
}   /* gui_run */



static void make_widgets(application_context)   /* ------------------------- */
XtAppContext	application_context ;
{
char		*new_chord, *new_scale ;
int		ndx ;
FretboardMode	initial_mode ;

/* Level 1 */
WC(PAN, Frame,	 	"frame",	ApplicationShell) ;
PC(TRN, HelpPopup,	"helpPopup",	ApplicationShell) ;


/* Level 2 -- children of Frame */
WC(FRT, Fretboard,	"fretboard",	Frame) ;
WC(BOX, Emacs1,		"emacs1",	Frame) ;
WC(BOX, Emacs2,		"emacs2",	Frame) ;
WC(BOX, Keys,		"keys",		Frame) ;
WC(BOX, Chords,		"chords",	Frame) ;
WC(BOX, Scales,		"scales",	Frame) ;
WC(BOX, Spelling,	"spelling",	Frame) ;
WC(BOX, Tunings,	"tunings",	Frame) ;

CB(Fretboard,	XtNnoteCallback,	noteCB,		NULL) ;
CB(Fretboard,	XtNmodeCallback,	externalModeCB,	NULL) ;


/* Level 2 -- children of HelpPopup */
WC(FRM,	HelpForm,	"helpForm",	HelpPopup) ;


/* Level 3 -- children of Emacs1 */
WC(LBL,	Emacs1Title,	"emacs1Title",		Emacs1) ;
WC(TOG, Chord,		"chord",		Emacs1) ;
WC(TOG, Lead,		"lead",			Emacs1) ;

CB(Chord,	XtNcallback,	internalModeCB,	NULL) ;
CB(Lead,	XtNcallback,	internalModeCB,	NULL) ;

XawToggleChangeRadioGroup(Chord, Lead) ;
XtVaSetValues(Chord, XtNradioData, ChordMode, NULL, NULL) ;
XtVaSetValues(Lead,  XtNradioData, LeadMode,  NULL, NULL) ;

#define CC(WIDGET, NAME, STRING) \
WC(BUT, WIDGET, NAME, Emacs1) ; \
CB(WIDGET, XtNcallback, commandCB, STRING) ; \
if (!application_resources.emacs) XtSetSensitive(WIDGET, False)

CC(NewStaff,	"newStaff",	"(tab-make-staff)") ;
CC(Barline,	"barline",	"(tab-barline)") ;
CC(HammerOn,	"hammerOn",	"(tab-hammer)") ;
CC(PullOff,	"pullOff",	"(tab-pull)") ;
CC(Bend,	"bend",		"(tab-bend)") ;
CC(Release,	"release",	"(tab-release)") ;
CC(SlideUp,	"slideUp",	"(tab-slide-up)") ;
CC(SlideDown,	"slideDown",	"(tab-slide-down)") ;
CC(GhostNote,	"ghostNote",	"(tab-ghost)") ;
CC(MuffledNote,	"muffledNote",	"(tab-muffled)") ;
CC(NormalNote,	"normalNote",	"(tab-normal)") ;
#undef CC


/* Level 3 -- children of Emacs2 */
WC(LBL,	Emacs2Title,	"emacs2Title",		Emacs2) ;

#define CC(WIDGET, NAME, STRING) \
WC(BUT, WIDGET, NAME, Emacs2) ; \
CB(WIDGET, XtNcallback, commandCB, STRING) ; \
if (!application_resources.emacs) XtSetSensitive(WIDGET, False)

CC(BackBar,	"backBar",	"(tab-backward-barline)") ;
CC(BackNote,	"backNote",	"(tab-backward-char 1)") ;
CC(FwrdNote,	"fwrdNote",	"(tab-forward)") ;
CC(FwrdBar,	"fwrdBar",	"(tab-forward-barline)") ;
CC(UpStaff,	"upStaff",	"(tab-up-staff 1)") ;
CC(UpNote,	"upNote",	"(previous-line 1)") ;
CC(DownNote,	"downNote",	"(next-line 1)") ;
CC(DownStaff,	"downStaff",	"(tab-down-staff 1)") ;
CC(PreviousNote,"previousNote",	"(tab-delete-chord-backward 1)") ;
CC(DeleteNote,	"deleteNote",	"(tab-delete-current-note)") ;
CC(SqueezeChord,"squeezeChord",	"(tab-delete-chord-forward 1)") ;
CC(InsertSpace,	"insertSpace",	"(tab-insert 1)") ;
#undef CC


/* Level 3 -- children of Keys */
WC(LBL, KeyTitle,	"keyTitle",		Keys) ;

#define CC(WIDGET, NAME, STRING) \
WC(TOG, WIDGET, NAME, Keys) ; \
CB(WIDGET, XtNcallback, keyCB, STRING) ; \
XtVaSetValues(WIDGET, XtNradioData, STRING, NULL, NULL)

CC(EKey,	"eKey",		0) ;
#undef CC

#define CC(WIDGET, NAME, STRING) \
WC(TOG, WIDGET, NAME, Keys) ; \
CB(WIDGET, XtNcallback, keyCB, STRING) ; \
XawToggleChangeRadioGroup(WIDGET, EKey) ; \
XtVaSetValues(WIDGET, XtNradioData, STRING, NULL, NULL)

CC(FKey,	"fKey",		1) ;
CC(FsharpKey,	"fsharpKey",	2) ;
CC(GKey,	"gKey",		3) ;
CC(AflatKey,	"aflatKey",	4) ;
CC(AKey,	"aKey",		5) ;
CC(BflatKey,	"bflatKey",	6) ;
CC(BKey,	"bKey",		7) ;
CC(CKey,	"cKey",		8) ;
CC(CsharpKey,	"csharpKey",	9) ;
CC(DKey,	"dKey",		10) ;
CC(EflatKey,	"eflatKey",	11) ;
#undef CC


/* Level 3 -- children of Chords and Scales */
WC(LBL, ChordTitle,	"chordTitle",		Chords) ;
WC(LBL, ScaleTitle,	"scaleTitle",		Scales) ;

number_of_chords  = count_tokens(application_resources.chords) ;
number_of_scales += count_tokens(application_resources.scales) ;

chord_scales = (ChordScaleDescript *)malloc(  (  number_of_chords 
					       + number_of_scales
					       + 1)
					    * sizeof(ChordScaleDescript)) ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
	strcpy(chord_scales[0].notes[ndx], chromatic_scale.notes[ndx]) ;

make_chord_scales(application_resources.chords, Chords, 1) ;
make_chord_scales(application_resources.scales, Scales, number_of_chords + 1) ;


/* Level 3 -- children of Spelling */
WC(LBL, SpellingTitle,	"spellingTitle",	Spelling) ;

ndx = 0 ;

#define CC(WIDGET, NAME, DEGREE) \
WC(TOG, WIDGET, NAME, Spelling) ; \
CB(WIDGET, XtNcallback, spellingCB, DEGREE) ; \
IntervalWidgets[ndx++] = WIDGET

CC(Interval0,	"interval0",	0) ;
CC(Interval1,	"interval1",	1) ;
CC(Interval2,	"interval2",	2) ;
CC(Interval3,	"interval3",	3) ;
CC(Interval4,	"interval4",	4) ;
CC(Interval5,	"interval5",	5) ;
CC(Interval6,	"interval6",	6) ;
CC(Interval7,	"interval7",	7) ;
CC(Interval8,	"interval8",	8) ;
CC(Interval9,	"interval9",	9) ;
CC(Interval10,	"interval10",	10) ;
CC(Interval11,	"interval11",	11) ;
#undef CC


/* Level 3 -- children of Tunings */
WC(LBL, TuningTitle, 	"tuningTitle",		Tunings) ;
WC(TXT, TuningText,	"tuningText",		Tunings) ;

XtVaGetValues(TuningText, XtNtextSource, &TuningTextSrc, NULL, NULL) ;
CB(TuningTextSrc, XtNcallback, tuningCB, NULL) ;


#define CC(WIDGET, NAME, CODE) \
WC(TOG, WIDGET, NAME, Tunings) ; \
CB(WIDGET, XtNcallback, intervalModeCB, CODE) ; \
XtVaSetValues(WIDGET, XtNradioData, CODE, NULL, NULL)

CC(ModeNormal,		"modeNormal",		NAMES_NORMAL) ;
CC(Mode12Tone,		"mode12Tone",		NAMES_12TONE) ;
#undef CC

XawToggleChangeRadioGroup(Mode12Tone, ModeNormal) ;

WC(BUT,	Quit,		"quit",			Tunings) ;
WC(BUT,	Help,		"help",			Tunings) ;

CB(Quit,	XtNcallback,	exitCB,		NULL) ;
CB(Help,	XtNcallback,	popupCB,	HelpPopup) ;



/* Level 3 -- children of HelpForm */
WC(TXT, HelpText,	"helpText",	HelpForm) ;
WC(BUT, HelpOkay,	"helpOkay",	HelpForm) ;

XtVaSetValues(HelpOkay, XtNfromVert, HelpText, NULL, NULL) ;

CB(HelpOkay,	XtNcallback,	popdownCB,	HelpPopup) ;



/* find out mode */
XtVaGetValues(Fretboard, XtNmode, &initial_mode, NULL, NULL) ;
externalModeCB(Fretboard, NULL, initial_mode) ;

}   /* make_widgets() */



/* Callbacks =============================================================== */

static void exitCB(widget, client_data, call_data)
Widget	widget ;
caddr_t	client_data, call_data ;
{
exit(0) ;
}   /* exitCB() */



static void noteCB(widget, client_data, note_info)
Widget			widget ;
caddr_t			client_data ;
FretboardCallbackStruct	*note_info ;
{
int		ndx, string ;
static	char	*string_names[] = {"e", "B", "G", "D", "A", "E"} ;

	for (ndx = 0 ; ndx < note_info->number_of_notes ; ndx++)
	fprintf(output,
		"(tab-%s-fret %d)",
		string_names[note_info->notes[ndx].string],
		note_info->notes[ndx].fret) ;

	if (global_mode == ChordMode)
	fprintf(output, "(tab-forward)\n") ;
	else
	fprintf(output, "\n") ;

}   /* noteCB() */



static void commandCB(widget, command_string, call_data)
Widget	widget ;
char	*command_string ;
caddr_t	call_data ;
{
fprintf(output, "%s\n", command_string) ;
}   /* commandCB() */



static Boolean internal_changed_mode = True ;

static void internalModeCB(widget, client_data, call_data)
Widget	widget ;
caddr_t	client_data, call_data ;
{
FretboardMode	new_mode ;
Boolean		state ;

XtVaGetValues(widget, XtNstate, &state, NULL, NULL) ;

	if (state == False)  return ;

new_mode = (widget == Lead ? LeadMode : ChordMode) ;

	if (new_mode == global_mode)  return ;

global_mode = new_mode ;

	switch (global_mode) {
	case LeadMode:
	fprintf(output, "(lead-mode)\n")  ;
	break ;

	case ChordMode:
	fprintf(output, "(chord-mode)\n") ;
	break ;
	}	

	if (internal_changed_mode)
	XtVaSetValues(Fretboard, XtNmode, global_mode, NULL, NULL) ;
	else
	internal_changed_mode = True ;

}   /* internalModeCB() */



static void externalModeCB(widget, client_data, new_mode)
Widget		widget ;
caddr_t		client_data ;
FretboardMode	new_mode ;
{

internal_changed_mode = False ;

	switch (new_mode) {
	case LeadMode:
	XawToggleSetCurrent(Lead, (caddr_t)LeadMode) ;
	break ;
	
	case ChordMode:
	XawToggleSetCurrent(Chord, (caddr_t)ChordMode) ;
	break ;
	}	
}   /* externalModeCB() */



static void keyCB(widget, key_number, call_data)
Widget	widget ;
char	*key_number ;
caddr_t	call_data ;
{
Boolean		state ;

XtVaGetValues(widget, XtNstate, &state, NULL, NULL) ;

	if (state == True)
	label_fretboard() ;

}   /* keyCB() */



static void chordScaleCB(widget, chordscale_ndx, call_data)
Widget	widget ;
int	chordscale_ndx ;
caddr_t	call_data ;
{
Boolean	state, note_state ;
int	ndx ;

XtVaGetValues(widget, XtNstate, &state, NULL, NULL) ;

	if (state != True || chordscale_ndx == -1)  return ;

label_degree_names(chordscale_ndx) ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++) {
		if (   strcmp(chord_scales[chordscale_ndx].notes[ndx], NO_NOTE)
		    == SAME)
		note_state = False ;
		else
		note_state = True ;

	XtVaGetValues(IntervalWidgets[ndx], XtNstate, &state, NULL, NULL) ;
	
		if (state != note_state)
		XtVaSetValues(IntervalWidgets[ndx],
			      XtNstate,	note_state,
			      NULL,	NULL) ;
	}

label_fretboard() ;

}   /* chordScaleCB() */



static void spellingCB(widget, degree, call_data)
Widget	widget ;
char	degree ;
caddr_t	call_data ;
{
Boolean		state ;

XtVaGetValues(widget, XtNstate, &state, NULL, NULL) ;

relative_chord_scale[degree] = state ;

label_fretboard() ;

}   /* spellingCB() */



static void intervalModeCB(widget, intervalMode_code, call_data)
Widget	widget ;
int	intervalMode_code ;
caddr_t	call_data ;
{
Boolean		state ;

XtVaGetValues(widget, XtNstate, &state, NULL, NULL) ;

	if (state) {
	label_degree_names(0) ;
	label_fretboard() ;
	}

}   /* intervalModeCB() */



static void tuningCB(widget, client_data, call_data)
Widget	widget ;
caddr_t	client_data, call_data ;
{
String	text ;
char	*note ;
int	numeric,
	ndx,
	new_tuning[NUMBER_OF_STRINGS],
	number_parsed = 0,
	string ;

XtVaGetValues(TuningTextSrc, XtNstring, &text, NULL, NULL) ;

	for (note = safe_strtok(text, TOKEN_SEPARATOR), string = 0 ;
	     note != NULL && string < NUMBER_OF_STRINGS ;
	     note = safe_strtok(NULL, TOKEN_SEPARATOR)) {
			
		if ((numeric = name_to_number(note)) != -1) {
		new_tuning[string] = numeric ;
		
		sprintf(&tuning_text[  string
				     * (FRETBOARD_MAX_LABEL_LENGTH + 1)],
			"%.*s",
			FRETBOARD_MAX_LABEL_LENGTH,
			note) ;

		number_parsed++ ;
		}
		else
		new_tuning[string] = 0 ;

	string++ ;
	}

	if (number_parsed == NUMBER_OF_STRINGS) {
	char	text[NUMBER_OF_STRINGS * 3 + 1],
		escaped_quotes[NUMBER_OF_STRINGS * 4 + 1],
		*textptr,
		*primeptr,
		*note_name,
		*in,
		*out ;
	int	repeated_notes[MAX_NOTES],
		note_number ;

		for (ndx = 0 ; ndx < NUMBER_OF_STRINGS ; ndx++)
		numeric_tuning[ndx] = new_tuning[ndx] ;

	label_fretboard() ;
	FretboardSetTuning(Fretboard, tuning_text) ;

	text[0] = '\0' ;
		for (ndx = 0 ; ndx < NUMBER_OF_STRINGS ; ndx++)
		strcat(text, "--|") ;

		for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
		repeated_notes[ndx] = 0 ;

		for (ndx = 0, textptr = text ;
		     ndx < NUMBER_OF_STRINGS ;
		     ndx++, textptr += 3) {

		note_number = numeric_tuning[ndx] ;
		note_name = number_to_name(note_number) ;
		strncpy(textptr, note_name, strlen(note_name)) ;
		*textptr = tolower(*textptr) ;

		primeptr = textptr + 1 ;
			if (*primeptr != '-')  primeptr++ ;

			switch (repeated_notes[note_number]) {
			case 0:
			*textptr = toupper(*textptr) ;			
			break ;

			case 1:
			/* already okay */
			break ;

			case 2:
			*primeptr = '\'' ;
			break ;
			
			case 3:
			*primeptr = '"' ;   /* '"' for emacs balanced-paren */
			break ;
			
			default:
			*primeptr = '5' ;
			*primeptr += repeated_notes[note_number] - 4 ;
			}

		repeated_notes[note_number]++ ;
		}

   		for (in = text, out = escaped_quotes ; ; ) {
			if (*in == '"'   /* '"' for emacs balanced-paren */ )
			*out++ = '\\' ;

		*out++ = *in ;
			if (*in++ == '\0')  break ;
		}
	
	fprintf(output, "(tab-set-tuning \"%s\")\n", escaped_quotes) ;
	}

}   /* tuningCB() */



static void popupCB(widget, popup, call_data)
Widget	widget, popup ;
caddr_t	call_data ;
{
Window		root, child ;
int		root_x,
		root_y,
		window_x,
		window_y,
		x,
		y,
		screen_width,
		screen_height ;
unsigned int	mask ;
Dimension	width, height ;

XtRealizeWidget(popup) ;

XQueryPointer(XtDisplay(popup),
	      XtWindow(popup),
	      &root,
	      &child,
	      &root_x,
	      &root_y,
	      &window_x,
	      &window_y,
	      &mask) ;
XtVaGetValues(popup,
	      XtNwidth,		&width,
	      XtNheight,	&height,
	      NULL,		NULL) ;

screen_width = WidthOfScreen(XtScreen(popup)) ;
x = root_x - width / 2 ;
	if (x + (int)width >= screen_width)  x = screen_width - width ;
	if (x < 0)  x = 0 ;

screen_height = HeightOfScreen(XtScreen(popup)) ;
y = root_y - height / 2 ;
	if (y + (int)height >= screen_height)  y = screen_height - height ;
	if (y < 0)  y = 0 ;

XtVaSetValues(popup,
	      XtNx,	x,
	      XtNy,	y,
	      NULL,	NULL) ;

XtPopup(popup, XtGrabNone) ;
}   /* popupCB() */



static void popdownCB(widget, popdown, call_data)
Widget	widget, popdown ;
caddr_t	call_data ;
{
XtPopdown(popdown) ;
}   /* popdownCB() */



/* private routines ======================================================== */
static void label_fretboard() {
int	ndx, key, string, fret, note, absolute_note, relative_note ;
Boolean	state ;
char	*note_label ;

	for (ndx = 0 ; ndx < NUMBER_OF_STRINGS ; ndx++)
	memset(string_fret_labels[ndx],
	       '\0',
	       (number_of_frets + 1) * (FRETBOARD_MAX_LABEL_LENGTH + 1)) ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++) {
	XtVaGetValues(IntervalWidgets[ndx], XtNstate, &state, NULL, NULL) ;
	relative_chord_scale[ndx] = state ;
	}

key = (int)XawToggleGetCurrent(EKey) ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
	absolute_chord_scale[ndx] = FALSE ;

	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
		if (relative_chord_scale[ndx]) {
		absolute_note = (ndx + key) % MAX_NOTES ;
		absolute_chord_scale[absolute_note] = TRUE ;
		}

	for (string = 0 ; string < NUMBER_OF_STRINGS ; string++) {
	note        = numeric_tuning[string] ;
	note_label = string_fret_labels[string] ;

		for (fret = 0 ; fret < number_of_frets + 1 ; fret++) {
			if (absolute_chord_scale[note]) {
			relative_note = (note - key + 12) % 12 ;
			strcpy(note_label, degree_names[relative_note]) ;
			}
		note++ ;
		note %= 12 ;
		note_label += FRETBOARD_MAX_LABEL_LENGTH + 1 ;
		}
	}

FretboardSetLabels(Fretboard, string_fret_labels) ;

}   /* label_fretboard() */



static void label_degree_names(chordscale_ndx)
int	chordscale_ndx ;
{
int			ndx ;
ChordScaleDescript	*chord_scale ;
char			name[FRETBOARD_MAX_LABEL_LENGTH + 1],
			*old_name ;

	if ((int)XawToggleGetCurrent(ModeNormal) == NAMES_12TONE) {
	/* just put 0-11 numbers in names */
		for (ndx = 0 ; ndx < MAX_NOTES ; ndx++) {
		sprintf(name, "%d", ndx) ;
		strcpy(degree_names[ndx], name) ;

		XtVaGetValues(IntervalWidgets[ndx],
			      XtNlabel,	&old_name,
			      NULL,	NULL) ;

			if (strcmp(old_name, name) != SAME)
			XtVaSetValues(IntervalWidgets[ndx],
				      XtNlabel,	name,
				      NULL,	NULL) ;
		}
	return ;
	}

/* put names in for ones current chord/scale doesn't specify */
chord_scale = &chord_scales[0] ;
	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
	strcpy(degree_names[ndx], chord_scale->notes[ndx]) ;

/* put in chord/scale specific names */
chord_scale = &chord_scales[chordscale_ndx] ;
	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++)
		if (strcmp(chord_scale->notes[ndx], NO_NOTE) != SAME)
		strcpy(degree_names[ndx], chord_scale->notes[ndx]) ;

/* label note widgets */
	for (ndx = 0 ; ndx < MAX_NOTES ; ndx++) {
	XtVaGetValues(IntervalWidgets[ndx],
		      XtNlabel,	&old_name,
		      NULL,	NULL) ;

		if (strcmp(old_name, degree_names[ndx]) != SAME)
		XtVaSetValues(IntervalWidgets[ndx],
			      XtNlabel,	degree_names[ndx],
			      NULL,	NULL) ;
	}

}   /* label_degree_names() */



/* Lower and uppercase because strcasecmp() is non-portable.  Lowercase
   first so number_to_name() finds them, and tuningCB() multiple-note
   logic works. */
static struct note_and_number {
	char	*name ;
	int	value ;
} notes_and_numbers[] = {
	{"e",	 0},
	{"f",	 1},
	{"f#",	 2},
	{"gb",	 2},
	{"g",	 3},
	{"g#",	 4},
	{"ab",	 4},
	{"a",	 5},
	{"a#",	 6},
	{"bb",	 6},
	{"b",	 7},
	{"c",	 8},
	{"c#",	 9},
	{"db",	 9},
	{"d",	10},
	{"d#",	11},
	{"eb",	11},
	{"E",	 0},
	{"F",	 1},
	{"F#",	 2},
	{"Gb",	 2},
	{"G",	 3},
	{"G#",	 4},
	{"Ab",	 4},
	{"A",	 5},
	{"A#",	 6},
	{"Bb",	 6},
	{"B",	 7},
	{"C",	 8},
	{"C#",	 9},
	{"Db",	 9},
	{"D",	10},
	{"D#",	11},
	{"Eb",	11} } ;


static int name_to_number(name)
char	*name ;
{
int	ndx ;

	for (ndx = 0 ;
	     ndx < sizeof(notes_and_numbers) / sizeof(struct note_and_number) ;
	     ndx++)
		if (strcmp(name, notes_and_numbers[ndx].name) == SAME)
		break ;

	if (ndx < sizeof(notes_and_numbers) / sizeof(struct note_and_number))
	return  notes_and_numbers[ndx].value ;
	else
	return  -1 ;

}   /* name_to_number() */



static char *number_to_name(number)
int	number ;
{
int	ndx ;

	for (ndx = 0 ;
	     ndx < sizeof(notes_and_numbers) / sizeof(struct note_and_number) ;
	     ndx++)
		if (notes_and_numbers[ndx].value == number)
		break ;

	if (ndx < sizeof(notes_and_numbers) / sizeof(struct note_and_number))
	return  notes_and_numbers[ndx].name ;
	else
	return  "e" ;

}   /* number_to_name() */



static char *safe_strtok(string, separators)
char	*string, *separators ;
{
static char	*safe_string = NULL,
		*token  ;

	if (string != NULL) {
		if (safe_string != NULL)  free(safe_string) ;
	safe_string = malloc(strlen(string) + 1) ;
	strcpy(safe_string, string) ;
	token = strtok(safe_string, separators) ;
	}
	else
	token = strtok(NULL, separators) ;

return  token ;

}   /* safe_strtok */



static int count_tokens(string)
char	*string ;
{
int	count = 0 ;
char	*token ;

	for (token = safe_strtok(string, TOKEN_SEPARATOR) ;
	     token != NULL ;
	     token = safe_strtok(NULL, TOKEN_SEPARATOR), count++)
	;

return  count ;

}   /* count_tokens() */



static void make_chord_scales(widget_list, parent, chord_scale_ndx)
char	*widget_list ;
Widget	parent ;
int	chord_scale_ndx ;
{
int		ndx, name_ndx, number_of_widgets ;
char		**widget_names, *widget_name, *note ;
Widget		widget ;
static Widget	first_widget = NULL ;

number_of_widgets = count_tokens(widget_list) ;
widget_names = (char **)malloc(number_of_widgets * sizeof(char *)) ;

	for (widget_name  = safe_strtok(widget_list, TOKEN_SEPARATOR), ndx = 0;
	     widget_name != NULL ;
	     widget_name  = safe_strtok(NULL, TOKEN_SEPARATOR), ndx++) {
	widget_names[ndx] = malloc(strlen(widget_name) + 1) ;
	strcpy(widget_names[ndx], widget_name) ;
	}

	for (name_ndx = 0 ;
	     name_ndx < number_of_widgets ;
	     name_ndx++, chord_scale_ndx++) {

	widget_name = widget_names[name_ndx] ;

	WC(TOG, widget, widget_name, parent) ;
	CB(widget, XtNcallback, chordScaleCB, chord_scale_ndx) ;

		if (first_widget == NULL)
		first_widget = widget ;
		else
		XawToggleChangeRadioGroup(widget, first_widget) ;

	XtGetSubresources(ApplicationShell,
			  (XtPointer)&spelling_resources,
			  widget_name,
			  widget_name,
			  spellingResources,
			  XtNumber(spellingResources),
			  NULL,
			  0) ;

		for (  note = safe_strtok(spelling_resources.spelling,
					  TOKEN_SEPARATOR)
		     , ndx = 0 ;
		     note != NULL ;
		     note = safe_strtok(NULL, TOKEN_SEPARATOR), ndx++)
		strncpy(chord_scales[chord_scale_ndx].notes[ndx],
			note,
			FRETBOARD_MAX_LABEL_LENGTH) ;
	}

	for (name_ndx = 0 ; name_ndx < number_of_widgets ; name_ndx++)
	free(widget_names[name_ndx]) ;
free(widget_names) ;

}   /* make_chord_scales() */
