/* fstPedNoise.h */
/* This file was made by the idl compiler "stic". Do not edit.
** This was generated for version '(unspecified)'
** Instead, edit the source idl file and then re-run the compiler.
** For help, type contact Craig Tull or Herb Ward. */
/* COMMENTS FROM IDL FILE:
 fstPedNoise.idl

 Table: fstPedNoise

 description: FST detector channel pedestal, rms noise and common mode noise


 */
#ifndef FSTPEDNOISE_H
#define FSTPEDNOISE_H
#define FSTPEDNOISE_SPEC \
"struct fstPedNoise { \
	unsigned short cmNoise[10368]; \
	unsigned short pedestal[331776]; \
	unsigned short totNoise[331776]; \
	unsigned short ranNoise[331776]; \
};"
typedef struct fstPedNoise_st {
	unsigned short cmNoise[10368]; /* 100 * cm noise in ADC counts per chip      */
	unsigned short pedestal[331776]; /* pedestal in ADC counts per channel   */
	unsigned short totNoise[331776]; /* 100 * total noise in ADC counts per channel  */
	unsigned short ranNoise[331776]; /* 100 * random noise in ADC counts per channel  */
} FSTPEDNOISE_ST;
#endif /* FSTPEDNOISE_H */
