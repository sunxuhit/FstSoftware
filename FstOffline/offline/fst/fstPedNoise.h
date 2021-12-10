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
	unsigned short cmNoise[288]; \
	unsigned short pedestal[36864]; \
	unsigned short rmsNoise[36864]; \
};"
typedef struct fstPedNoise_st {
	unsigned short cmNoise[288]; /* 100 * cm noise in ADC counts per chip      */
	unsigned short pedestal[36864]; /* pedestal in ADC counts per channel   */
	unsigned short rmsNoise[36864]; /* 100 * rms noise in ADC counts per channel  */
} FSTPEDNOISE_ST;
#endif /* FSTPEDNOISE_H */
