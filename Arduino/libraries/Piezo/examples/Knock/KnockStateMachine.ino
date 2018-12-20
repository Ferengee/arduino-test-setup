Vertex state_machine_links[9];

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
  maybePlayRecorded.on(l++, GO)->to(playRecorded);
  playRecorded.on(l++, LISTEN)->to(listening);

/* # functions */

/*    confirm the knock, reset the knock timer  */
  maybeGreen.enterfunc = confirmKnock;
  maybeRed.enterfunc = confirmKnock;
  maybePlayRecorded.enterfunc = confirmKnock;

/*  control traffic light state */
  green.enterfunc = lightGreen;
  red.enterfunc = lightRed;
  playRecorded.enterfunc = lightPlayRecorded;

/*  machines */
  machine.start();
}
