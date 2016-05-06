/*
	timer.h
	header file for timer class
	Stefan Seegel, post@seegel-systeme.de
	feb 2011
	last update apr 2016
	http://opensource.org/licenses/gpl-license.php GNU Public License
*/
#ifndef timer_h
#define timer_h

#include <avr/io.h>

#define AVRTIMERNUM	2
/* timer overview, see datasheets for details
	Timernum	width	prescalers
	0			8		1, 8, 64, 256, 1024
	1			16		1, 8, 64, 256, 1024
	2			8		1, 8, 32, 64, 128, 256, 1024
	3			16		1, 8, 64, 256, 1024
	4			16		1, 8, 64, 256, 1024
	5			16		1, 8, 64, 256, 1024
*/

#define TICKFREQUENCY 1000UL	//Tickfrequency of timerobjects in Hz


#if ((AVRTIMERNUM == 0) || (AVRTIMERNUM == 2))
	#define MAXTIMERVAL 0xFFL
#else
	#define MAXTIMERVAL 0xFFFFUL
#endif

#if (AVRTIMERNUM == 0)
	#ifndef OCIE0A
		#error Timer 0 has no OC feature for timer 0. Please select a different timer in timer.h!
	#endif
	#define TIMCOMPVECT TIMER0_COMPA_vect
	#define OCRREG OCR0A
	#define PRESCALEREG TCCR0B
	#define IRQFLAGBIT OCIE0A
#elif (AVRTIMERNUM == 2)
	#define TIMCOMPVECT TIMER2_COMP_vect
	#define OCRREG OCR2
	#define PRESCALEREG TCCR2
	#define IRQFLAGBIT OCIE2
#endif

#if (AVRTIMERNUM == 0)
	#if (F_CPU / TICKFREQUENCY) < MAXTIMERVAL
		#define CSVAL 1
		#define PRESCAL 1UL
	#elif (F_CPU / TICKFREQUENCY / 8) < MAXTIMERVAL
		#define CSVAL 2
		#define PRESCAL 8UL
	#elif (F_CPU / TICKFREQUENCY / 64) < MAXTIMERVAL
		#define CSVAL 3
		#define PRESCAL 64UL
	#elif (F_CPU / TICKFREQUENCY / 256) < MAXTIMERVAL
		#define CSVAL 4
		#define PRESCAL 256UL
	#elif (F_CPU / TICKFREQUENCY / 1024) < MAXTIMERVAL
		#define CSVAL 5
		#define PRESCAL 1024UL
	#endif
#elif (AVRTIMERNUM == 2)
	#if (F_CPU / TICKFREQUENCY) < MAXTIMERVAL
		#define CSVAL 1
		#define PRESCAL 1UL
	#elif (F_CPU / TICKFREQUENCY / 8) < MAXTIMERVAL
		#define CSVAL 2
		#define PRESCAL 8UL
	#elif (F_CPU / TICKFREQUENCY / 32) < MAXTIMERVAL
		#define CSVAL 3
		#define PRESCAL 32UL
	#elif (F_CPU / TICKFREQUENCY / 64) < MAXTIMERVAL
		#define CSVAL 4
		#define PRESCAL 64UL
	#elif (F_CPU / TICKFREQUENCY / 128) < MAXTIMERVAL
		#define CSVAL 5
		#define PRESCAL 128UL
	#elif (F_CPU / TICKFREQUENCY / 256) < MAXTIMERVAL
		#define CSVAL 6
		#define PRESCAL 256UL
	#elif (F_CPU / TICKFREQUENCY / 1024) < MAXTIMERVAL
		#define CSVAL 7
		#define PRESCAL 1024UL
	#endif
#else
	#error not yet implemented
#endif

#ifndef CSVAL
	#error Timer: wrong config values!
#endif
#define TOTALDIV (TICKFREQUENCY * PRESCAL)
#define OCRVAL (F_CPU / TICKFREQUENCY / PRESCAL - 1)

#include <avr/io.h>

extern volatile bool timers_idle;

#define STR(x) #x
#define HLPSTR(x) STR(x)

class Timer {
	private:
		uint16_t cnt;
		uint16_t loadval;
		bool oneshot;
		bool flag;
		static void irq() __asm__(HLPSTR(TIMCOMPVECT)) __attribute__((__signal__, __used__, __externally_visible__));
		void Init();
		Timer *prior;
		static Timer *lastobj;
	public:
		Timer(bool oneshot = true);
		Timer(uint16_t ticks, bool oneshot = true);
		bool IsFlagged(void);
		void SetTime(uint16_t ticks);
};

#undef STR
#undef HLPSTR

#endif
