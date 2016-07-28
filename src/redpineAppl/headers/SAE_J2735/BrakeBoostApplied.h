/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735.asn"
 */

#ifndef	_BrakeBoostApplied_H_
#define	_BrakeBoostApplied_H_


#include <SAE_J2735/asn_application.h>

/* Including external dependencies */
#include <SAE_J2735/NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BrakeBoostApplied {
	BrakeBoostApplied_unavailable	= 0,
	BrakeBoostApplied_off	= 1,
	BrakeBoostApplied_on	= 2
} e_BrakeBoostApplied;

/* BrakeBoostApplied */
typedef long	 BrakeBoostApplied_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BrakeBoostApplied;
asn_struct_free_f BrakeBoostApplied_free;
asn_struct_print_f BrakeBoostApplied_print;
asn_constr_check_f BrakeBoostApplied_constraint;
ber_type_decoder_f BrakeBoostApplied_decode_ber;
der_type_encoder_f BrakeBoostApplied_encode_der;
xer_type_decoder_f BrakeBoostApplied_decode_xer;
xer_type_encoder_f BrakeBoostApplied_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _BrakeBoostApplied_H_ */
#include <SAE_J2735/asn_internal.h>