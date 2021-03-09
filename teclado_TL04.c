#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	int i=0;
	int j=0;


	p_teclado->columna_actual= COLUMNA_1;
	//p_teclado->debounceTime = 0;
	p_teclado->teclaPulsada.col= -1;
	p_teclado->teclaPulsada.row=-1;
	p_teclado->flags=0;



	for(i=0; i<4; i++ ){
		pinMode (teclado.filas[i], INPUT);
		pullUpDnControl(teclado.filas[i], PUD_DOWN);
		wiringPiISR (teclado.filas[i], INT_EDGE_RISING, teclado.rutinas_ISR[i]);
		p_teclado->debounceTime [i]=0;
	}

	int j=0;
	for(j=0; i<4; j++ ){
		pinMode (teclado.columnas[j], OUTPUT);
		digitalWrite(teclado.columnas[j], LOW);
		}


	p_teclado->tmr_duracion_columna=tmr_new(timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado -> tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno

	tmr_startms((tmr_t*)(teclado -> tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

	switch(columna){
		// ...
	case COLUMNA_1:

			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_1, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);


	break;

	case COLUMNA_2:

			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);


	break;

	case COLUMNA_3:

			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);


	break;

	case COLUMNA_4:

			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_4, HIGH);


	break;
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno...
	pilock(KEYBOARD_KEY);
	result = p_teclado->flags & FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock(KEYBOARD_KEY);

	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	pilock(KEYBOARD_KEY);
	result = p_teclado->flags & FLAG_TECLA_PULSADA;
	//result = teclado.flags;
	piUnlock(KEYBOARD_KEY);


	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	piLock (KEYBOARD_KEY);
	teclado.flags &=(~FLAG_TIMEOUT_COLUMNA_TECLADO);
	p_teclado->columna_actual++;


	if(p_teclado->columna_actual>=4){
		p_teclado->columna_actual=0;
	}

	ActualizaExcitacionTecladoGPIO(p_teclado->columna_actual);
     piUnlock(KEYBOARD_KEY);
	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	piLock (KEYBOARD_KEY);
	teclado.flags &=(~FLAG_TECLA_PULSADA);

	switch(p_teclado->teclaPulsada.col){


	case COLUMNA_1:


		    if(p_teclado ->teclaPulsada.row == FILA_2){

					fflush(stdout);
					p_teclado -> teclaPulsada.row = -1;
					p_teclado->teclaPulsada.col = -1;
			}
		    break;

	case COLUMNA_2:

	}



//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	f(millis ()< teclado.debounceTime[FILA_1]){
		teclado.debounceTime[FILA_1] = millis ()+ DEBOUNCE_TIME ;
		return;
	}

	piLock (KEYBOARD_KEY);
		teclado.teclaPulsada.row = FILA_1;
		teclado.teclaPulsada.col = teclado.columna_actual;
		flags |= FLAG_TECLA_PULSADA;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_1]= millis ()+ DEBOUNCE_TIME ;

	}


}

void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...
	  if(millis ()< teclado.debounceTime[FILA_2]){
			teclado.debounceTime[FILA_2] = millis ()+ DEBOUNCE_TIME ;
			return;
		}

		piLock (KEYBOARD_KEY);
			teclado.teclaPulsada.row = FILA_2;
			teclado.teclaPulsada.col = teclado.columna_actual;
			flags |= FLAG_TECLA_PULSADA;
		piUnlock (KEYBOARD_KEY);

		teclado.debounceTime[FILA_2]= millis ()+ DEBOUNCE_TIME ;
}

void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
	if(millis ()< teclado.debounceTime[FILA_3]){
			teclado.debounceTime[FILA_3] = millis ()+ DEBOUNCE_TIME ;
			return;
		}

		piLock (KEYBOARD_KEY);
			teclado.teclaPulsada.row = FILA_3;
			teclado.teclaPulsada.col = teclado.columna_actual;
			teclado.flags |= FLAG_TECLA_PULSADA;
		piUnlock (KEYBOARD_KEY);

		teclado.debounceTime[FILA_3]= millis ()+ DEBOUNCE_TIME ;
}

void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
	if(millis ()< teclado.debounceTime[FILA_4]){
			teclado.debounceTime[FILA_4] = millis ()+ DEBOUNCE_TIME ;
			return;
		}

		piLock (KEYBOARD_KEY);
			teclado.teclaPulsada.row = FILA_4;
			teclado.teclaPulsada.col = teclado.columna_actual;
			teclado.flags |= FLAG_TECLA_PULSADA;
		piUnlock (KEYBOARD_KEY);

		teclado.debounceTime[FILA_4]= millis ()+ DEBOUNCE_TIME ;
}

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock (KEYBOARD_KEY);
}
