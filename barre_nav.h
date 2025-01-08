#ifndef APP_STATE_H
#define APP_STATE_H

// Variable pour suivre l'état actuel
// Déclaration des états de l'application
typedef enum {
    STATE_HOME,
    STATE_WELCOME,
    STATE_CALENDAR,
    STATE_CREATION,
    STATE_TENUES,
    STATE_QUIT 
} AppState;


#endif // APP_STATE_H
