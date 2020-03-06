static String fallback_resources[] = {
"*emacs:			True",
"*Box.background:	peach puff",
"*Label.background:	peach puff",
"*Label.borderWidth:	0",
"*Toggle.background:	white",
"*Command.background:	white",
"*Toggle.shapeStyle:	oval",
"*Command.shapeStyle:	rectangle",
"*font:			8x13",
"*frame.width:		820",
"*frame.height:		356",
"*helpPopup.width:	640",
"*helpPopup.height:	300",
"*helpPopup.title:	Help",
"*fretboard.background:		bisque",
"*fretboard.height:		160",
"*fretboard.mode:		chord",
"*fretboard.frets:		22",
"*fretboard.font:		8x13bold",
"*fretboard.strings:		6",
"*fretboard.orientation:		horizontal",
"*fretboard.stringWidths:	4w 3w 2w 2p 1p 1p",
"*fretboard.fretWidth:		5",
"*fretboard.nutWidth:		15",
"*fretboard.windingSize:		2",
"*fretboard.windingGap:		1",
"*fretboard.dotmarkerPercent:	70",
"*fretboard.notePercent:		85",
"*fretboard.fretboardColor:	brown4",
"*fretboard.leadCursorColor:	cyan",
"*fretboard.leadCursorBkgColor:	cyan",
"*fretboard.chordCursorColor:	green",
"*fretboard.chordCursorBkgColor:	green",
"*fretboard.fretColor:		gold",
"*fretboard.stringColor:		light gray",
"*fretboard.windingColor:	goldenrod",
"*fretboard.windingGapColor:	white",
"*fretboard.dotmarkerColor:	lavender",
"*fretboard.nutColor:		ivory",
"*fretboard.headstockColor:	goldenrod",
"*fretboard.noteColor:		red",
"*fretboard.marginTextColor:	light slate gray",
"*fretboard.fretboardTextColor:	white",
"*fretboard.headstockTextColor:	black",
"*fretboard.translations:	<Btn1Down>:	armNote()	\\n\
				<Btn1Up>:	makeNote()	\\n\
				<Btn2Down>:	sendChord() 	\\n\
				<Btn3Down>:	leadVsChord()",
"*helpForm*background:		old lace",
"*emacs1Title.label:		Emacs: ",
"*chord.label:			chord",
"*lead.label:			lead",
"*newStaff.label:		staff",
"*barline.label:			bar",
"*hammerOn.label:		hammer",
"*pullOff.label:			pull",
"*bend.label:			bend",
"*release.label:			release",
"*slideUp.label:			sld up",
"*slideDown.label:		sld dwn",
"*ghostNote.label:		ghost",
"*muffledNote.label:		muffle",
"*normalNote.label:		plain",
"*emacs.Toggle.translations:	<EnterWindow>:		highlight(Always)  \\n\
				<LeaveWindow>:		unhighlight()	   \\n\
				<Btn1Down>,<Btn1Up>:	set() notify()",
"*emacs2Title.label:		Emacs: ",
"*backBar.label:			<-bar",
"*backNote.label:		<-note",
"*fwrdNote.label:		note->",
"*fwrdBar.label:			bar->",
"*upStaff.label:			UP",
"*upNote.label:			up",
"*downNote.label:		down",
"*downStaff.label:		DOWN",
"*previousNote.label:		backspace",
"*deleteNote.label:		kill note",
"*squeezeChord.label:		squeeze",
"*insertSpace.label:		insert",
"*eKey.state:		True",
"*keyTitle.label:	Key:   ",
"*eKey.label:		E",
"*fKey.label:		F",
"*fsharpKey.label:	F#/Gb",
"*gKey.label:		G",
"*aflatKey.label:	Ab/G#",
"*aKey.label:		A",
"*bflatKey.label:	Bb/A#",
"*bKey.label:		B",
"*cKey.label:		C",
"*csharpKey.label:	C#/Db",
"*dKey.label:		D",
"*eflatKey.label:	Eb/D#",
"*keys.Toggle.translations:	<EnterWindow>:		highlight(Always)  \\n\
				<LeaveWindow>:		unhighlight()	   \\n\
				<Btn1Down>,<Btn1Up>:	set() notify()",
"*chordTitle.label:		Chord: ",
"*chordList:			chordRoot		\
				chord5			\
				chordMajor		\
				chordMinor		\
				chordSus2		\
				chordSus4		\
				chord7			\
				chordMinor7		\
				chordMajor7		\
				chord6			\
				chordMinor6		\
				chord9			\
				chordMinor9		\
				chordDiminished		\
				chord11			\
				chordMinor11		\
				chord13			\
				chordMinor13",
"*chordRoot.spelling:		rt   -   -   -   -   -   -   -   -   -   -   -",
"*chord5.spelling:		rt   -   -   -   -   -   -   5   -   -   -   -",
"*chordMajor.spelling:		rt   -   -   -   3   -   -   5   -   -   -   -",
"*chordMinor.spelling:		rt   -   -  b3   -   -   -   5   -   -   -   -",
"*chordSus2.spelling:		rt   -   2   -   -   -   -   5   -   -   -   -",
"*chordSus4.spelling:		rt   -   -   -   -   4   -   5   -   -   -   -",
"*chord7.spelling:		rt   -   -   -   3   -   -   5   -   -   7   -",
"*chordMinor7.spelling:		rt   -   -  b3   -   -   -   5   -   -   7   -",
"*chordMajor7.spelling:		rt   -   -   -   3   -   -   5   -   -   -   7",
"*chord6.spelling:		rt   -   -   -   3   -   -   5   -   6   -   -",
"*chordMinor6.spelling:		rt   -   -  b3   -   -   -   5   -   6   -   -",
"*chord9.spelling:		rt   -   9   -   3   -   -   5   -   -   7   -",
"*chordMinor9.spelling:		rt   -   9  b3   -   -   -   5   -   -   7   -",
"*chordDiminished.spelling:	rt   -   -  b3   -   -  b5   -   - bb7   -   -",
"*chord11.spelling:		rt   -   9   -   3  11   -   5   -   -   7   -",
"*chordMinor11.spelling:		rt   -   9  b3   -  11   -   5   -   -   7   -",
"*chord13.spelling:		rt   -   9   -   3  11   -   5   -  13   7   -",
"*chordMinor13.spelling:		rt   -   9  b3   -  11   -   5   -  13   7   -",
"*chordRoot.label:		root",
"*chord5.label:			5",
"*chordMajor.label:		major",
"*chordMinor.label:		minor",
"*chordSus2.label:		sus2",
"*chordSus4.label:		sus4",
"*chord7.label:			7",
"*chordMinor7.label:		min7",
"*chordMajor7.label:		maj7",
"*chord6.label:			6",
"*chordMinor6.label:		min6",
"*chord9.label:			9",
"*chordMinor9.label:		min9",
"*chordDiminished.label:		dim",
"*chord11.label:			11",
"*chordMinor11.label:		min11",
"*chord13.label:			13",
"*chordMinor13.label:		min13",
"*chords.Toggle.translations:	<EnterWindow>:		highlight(Always)  \\n\
				<LeaveWindow>:		unhighlight()	   \\n\
				<Btn1Down>,<Btn1Up>:	set() notify()",
"*scaleTitle.label:		Scale: ",
"*noScale.state:			True",
"*scaleList:			noScale			\
				chromaticScale		\
				majorScale		\
				minorScale		\
				bluesScale		\
				majorPentScale		\
				minorPentScale		\
				dorianMode		\
				phrygianMode		\
				lydianMode		\
				mixolydianMode		\
				aeolianMode		\
				locrianMode	",
"*noScale.spelling:		 -   -   -   -   -   -   -   -   -   -   -   -",
"*chromaticScale.spelling:	rt  b2   2  b3   3   4  b5   5  b6   6  b7   7",
"*majorScale.spelling:		rt   -   2   -   3   4   -   5   -   6   -   7",
"*minorScale.spelling:		rt   -   2  b3   -   4   -   5  b6   -  b7   -",
"*bluesScale.spelling:		rt   -   -  b3   -   4  b5   5   -   -  b7   -",
"*majorPentScale.spelling:	rt   -   2   -   3   -   -   5   -   6   -   -",
"*minorPentScale.spelling:	rt   -   -  b3   -   4   -   5   -   -  b7   -",
"*dorianMode.spelling:		rt   -   2  b3   -   4   -   5   -   6  b7   -",
"*phrygianMode.spelling:		rt  b2   -  b3   -   4   -   5  b6   -  b7   -",
"*lydianMode.spelling:		rt   -   2   -   3   -  b5   5   -   6   -   7 ",
"*mixolydianMode.spelling:	rt   -   2   -   3   4   -   5   -   6  b7   -",
"*aeolianMode.spelling:		rt   -   2  b3   -   4   -   5  b6   -  b7   -",
"*locrianMode.spelling:		rt  b2   -  b3   -   4  b5   -  b6   -  b7   -",
"*noScale.label:			none",
"*chromaticScale.label:		chrom",
"*majorScale.label:		major",
"*minorScale.label:		minor",
"*bluesScale.label:		blues",
"*majorPentScale.label:		maj pent",
"*minorPentScale.label:		min pent",
"*dorianMode.label:		dor",
"*phrygianMode.label:		phryg",
"*lydianMode.label:		lyd",
"*mixolydianMode.label:		mix",
"*aeolianMode.label:		aeol",
"*locrianMode.label:		locr",
"*scales.Toggle.translations:	<EnterWindow>:		highlight(Always)  \\n\
				<LeaveWindow>:		unhighlight()	   \\n\
				<Btn1Down>,<Btn1Up>:	set() notify()",
"*spellingTitle.label:		Notes: ",
"*tuningText*editType:		edit",
"*tuningText.resize:		width",
"*tuningText*string:		E A D G B e",
"*modeNormal.state:		True",
"*tuningTitle.label:		Other: ",
"*modeNormal.label:		Normal",
"*mode12Tone.label:		12 Tone",
"*quit.font:			8x13bold",
"*quit.label:			QUIT",
"*tunings.Toggle.translations:	<EnterWindow>:		highlight(Always)  \\n\
				<LeaveWindow>:		unhighlight()	   \\n\
				<Btn1Down>,<Btn1Up>:	set() notify()",
"*helpOkay.top:			chainBottom",
"*helpOkay.bottom:		chainBottom",
"*helpOkay.label:		Remove help",
"*helpText.bottom:		chainBottom",
"*helpText.right:		chainRight",
"*helpText*editType:		read",
"*helpText.scrollHorizontal:	whenNeeded",
"*helpText.scrollVertical:	whenNeeded",
"*helpText*string:		\
FRETBOARD (mouse buttons):\\n\
      left down:  Show note\\n\
        left up:  Make note, unless mouse has moved since \"left down\".\\n\
                      In lead mode, immediately send and remove note.\\n\
                      In chord mode, toggles notes on/off.\\n\
    middle down:  Send and remove notes (chord mode only)\\n\
     right down:  Toggle lead/chord mode.  Remove any chord notes,\\n\
                      without sending.\\n\
\\n\
\\n\
EMACS:\\n\
     chord/lead:  Set mode\\n\
          staff:  Make new tablature staff\\n\
            bar:  Make barline\\n\
         hammer:  Embellishments.  If cursor is on note, mark it with\\n\
           pull:      the embellishment.  Otherwise, mark next note\\n\
           bend:      entered with the embellishment. \\n\
        release:    \"\\n\
         sld up:    \"\\n\
       sld down:    \"\\n\
          ghost:    \"\\n\
         muffle:    \"\\n\
          plain:  Remove embellishment\\n\
          <-bar:  Move cursor to previous barline\\n\
         <-note:  Move cursor to previous note\\n\
         note->:  Move cursor to next note\\n\
          bar->:  Move cursor to previous barline\\n\
             UP:  Move cursor to previous tablature staff\\n\
             up:  Move cursor up one string\\n\
           down:  Move cursor down one string\\n\
           DOWN:  Move cursor to next tablature staff\\n\
      backspace:  Move cursor backwards, removing note(s).  Move all\\n\
                      following notes on staff one space backwards.\\n\
      kill note:  Remove note at cursor.\\n\
        squeeze:  Remove note(s) at cursor position.  Move all\\n\
                      following notes on staff one space backwards.\\n\
         insert:  Move notes at cursor position, and all following\\n\
                      notes, one space forward.\\n\
\\n\
\\n\
KEY:\\n\
    Key for chord/scale/note labeling of fretboard.\\n\
\\n\
\\n\
CHORD and SCALE:\\n\
    Chord or scale for labeling fretboard.  May be modified by notes\\n\
    in NOTES section.  Use \"none\" in SCALE section to turn off.\\n\
\\n\
\\n\
NOTES:\\n\
   Chord/scale notes for labeling fretboard.  May be set with CHORD\\n\
   or SCALE buttons, and/or changed directly.\\n\
\\n\
\\n\
OTHER:\\n\
         tuning:  String tuning, low-to-high.  Upper/lower case ignored,\\n\
                      will be changed before sending to emacs.  Use\\n\
                      letters \"A\" thru \"G\", and \"#\" and \"b\".  Tuning\\n\
                      will change if/when six correct notes; ignored\\n\
                      otherwise.\\n\
         normal:  Label fretboard with normal (rt, 5, b3, etc.) names\\n\
        12 Tone:  Label fretboard with chromatic (0 thru 11) names\\n\
           QUIT:  Exit program\\n\
           help:  Show this screen",
NULL
} ;
