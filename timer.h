/*
	timer.h
	header file for timer class
	Stefan Seegel, post@seegel-systeme.de
	feb 2011
	last update okt 2016
	http://opensource.org/licenses/gpl-license.php GNU Public License
*/
#ifndef timer_h
#define timer_h

#include <avr/io.h>

#ifndef AVRTIMERNUM
	#warning Please define symbol 'AVRTIMERNUM' (0-5) in project settings! Using default 0
	#define AVRTIMERNUM 0
#endif

/* timer overview, see datasheets for details
	Timernum	width	prescalers
	0			8		1, 8, 64, 256, 1024 or 1, 8, 32, 64, 128, 256, 1024
	1			16		1, 8, 64, 256, 1024
	2			8		1, 8, 64, 256, 1024 or 1, 8, 32, 64, 128, 256, 1024
	3			16		1, 8, 64, 256, 1024
	4			16		1, 8, 64, 256, 1024
	5			16		1, 8, 64, 256, 1024
*/

#ifndef TICKFREQUENCY
	#define TICKFREQUENCY 1000UL	//Tickfrequency of timerobjects in Hz
#endif


#if ((AVRTIMERNUM == 0) || (AVRTIMERNUM == 2))
	#define MAXTIMERVAL 0xFFL
#else
	#define MAXTIMERVAL 0xFFFFUL
#endif

#if (AVRTIMERNUM == 0)
	#if defined(OCIE0A)
		#define TIMCOMPVECT TIMER0_COMPA_vect
		#define OCRREG OCR0A
		#define PRESCALEREG TCCR0B
		#define IRQFLAGBIT OCIE0A
		#define IRQREG TIMSK0
	#elif defined(OCIE0)
		#define TIMCOMPVECT TIMER0_COMP_vect
		#define PRESCALEREG TCCR0
		#define OCRREG OCR0
		#define IRQREG TIMSK
		#define IRQFLAGBIT OCIE0
	#else
	#error Timer 0 has no OC feature for timer 0. Please select a different timer in timer.h!
	#endif
#elif (AVRTIMERNUM == 1)
	#error not yet implemented, please complain to author!
#elif (AVRTIMERNUM == 2)
	#ifdef TIMER2_COMP_vect
		#define IRQREG TIMSK
		#define TIMCOMPVECT TIMER2_COMP_vect
		#define OCRREG OCR2
		#define PRESCALEREG TCCR2
		#define IRQFLAGBIT OCIE2
	#else
	    #define IRQREG TIMSK2
		#define TIMCOMPVECT TIMER2_COMPA_vect
		#define OCRREG OCR2A
		#define PRESCALEREG TCCR2B
		#define IRQFLAGBIT OCIE2A
	#endif
#else
	#error AVRTIMERNUM not yet implemented, please use TIMERNUM 0 or 2
#endif

#if ( (AVRTIMERNUM == 0) && defined(AS0) ) || ( (AVRTIMERNUM == 2) && defined(AS2) )
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
		operator bool(); // {return IsFlagged();}
		void SetTime(uint16_t ticks);
};

#undef STR
#undef HLPSTR

#endif
