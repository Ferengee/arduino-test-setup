Vertex state_machine_links[17];

void setup_machines(){
  Vertex * l = state_machine_links;

/* # knock once */
  listening.on(l++, KNOCK)->to(maybeGreen);
  maybeGreen.on(l++, GO)->to(green);
  green.on(l++, LISTEN)->to(listening);

/* # knock twice */

/*    listening-KNOCK->maybeGreen */
  maybeGreen.on(l++, KNOCK)->to(maybeRed);
  maybeRed.on(l++, GO)->to(red);
  red.on(l++, LISTEN)->to(listening);

/* # knock three times */

/*    listening-KNOCK->maybeGreen */

/*    maybeGreen-KNOCK->maybeRed */
  maybeRed.on(l++, KNOCK)->to(maybePlayRecorded);
  maybePlayRecorded.on(l++, GO)->to(autoGreen);

/* # knock four times */

/*    listening-KNOCK->maybeGreen */

/*    maybeGreen-KNOCK->maybeRed */

/*    maybeRed-KNOCK->maybePlayRecorded */
  maybePlayRecorded.on(l++, KNOCK)->to(maybePlayMorse);
  maybePlayMorse.on(l++, GO)->to(playMorse);
  playMorse.on(l++, LISTEN)->to(listening);

/* # automatic light change */

/*  confirmKnock has to stop ProgressTimer */

/*  ProgressTimer must be reset in enterfuncs for auto states */
  autoRed.on(l++, KNOCK)->to(listening);
  autoYellow.on(l++, KNOCK)->to(listening);
  autoGreen.on(l++, KNOCK)->to(listening);
  autoGreen.on(l++, PROGRESS)->to(autoYellow);
  autoYellow.on(l++, PROGRESS)->to(autoRed);
  autoRed.on(l++, PROGRESS)->to(autoGreen);

/* # functions */

/*    confirm the knock, reset the knock timer  */
  maybeGreen.enterfunc = confirmKnock;
  maybeRed.enterfunc = confirmKnock;
  maybePlayRecorded.enterfunc = confirmKnock;
  maybePlayMorse.enterfunc = confirmKnock;

/*  control traffic light state */
  green.enterfunc = lightGreen;
  red.enterfunc = lightRed;
  playMorse.enterfunc = lightPlayMorse;
  autoRed.enterfunc = lightRedAndResetTimer;
  autoYellow.enterfunc = lightYellowAndResetTimer;
  autoGreen.enterfunc = lightGreenAndResetTimer;

/*  machines */
  machine.start();
}
